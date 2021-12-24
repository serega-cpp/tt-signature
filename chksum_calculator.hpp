#ifndef __CHKSUM_CALCULATOR_HPP__
#define __CHKSUM_CALCULATOR_HPP__

#include <fstream>
#include <vector>
#include <queue>

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "file.hpp"
#include "memory_pool.hpp"
#include "memory_block.hpp"
#include "chksum.hpp"

class ChksumCalculator
{
public:
    ChksumCalculator(size_t block_size, size_t blocks_count)
        : _block_size(block_size)
        , _mempool(block_size)
        , _chksums(blocks_count)
        , _processed_bytes(0)
        , _terminate_producer(false)
        , _terminate_consumer(false)
    {
    }

    void BlocksConsumer()
    {
        for (;;) {
            std::unique_lock<std::mutex> lock(_mx);

            _cv_block_added.wait(lock, [this](){
                return !_blocks.empty() || _terminate_consumer;
            });

            if (_terminate_consumer)
                break;

            auto block = std::move(_blocks.front());
            _blocks.pop();
            _cv_block_removed.notify_all();

            lock.unlock();

            _chksums[block.id] = CalcChkSum(block.ptr.get(), block.size);
            _mempool.Put(std::move(block.ptr));

            _processed_bytes += block.size;
        }
    }

    void BlocksProducer(int fd, size_t prefetch_count)
    {
        size_t block_idx = 0;

        for (;;) {
            auto ptr = _mempool.Get();

            int size = read(fd, ptr.get(), static_cast<unsigned int>(_block_size));
            if (size <= 0)
                break; // eof or error
        
            std::unique_lock<std::mutex> lock(_mx);

            _blocks.emplace(std::move(ptr), size, block_idx++);
            _cv_block_added.notify_one();

            _cv_block_removed.wait(lock, [this, prefetch_count](){
                return _blocks.size() < prefetch_count || _terminate_producer;
            });

            if (_terminate_producer)
                break;
        }

        Stop(); // gracefuly stops consumers
    }

    void Stop()
    {
        std::unique_lock<std::mutex> lock(_mx);

        _terminate_producer = true;
        _cv_block_removed.notify_one();

        if (!_blocks.empty()) {
            _cv_block_removed.wait(lock, [this](){
                return _blocks.empty();
            });
        }

        _terminate_consumer = true;
        _cv_block_added.notify_all();
    }

    size_t GetProcessedBytes()
    {
        return _processed_bytes.load();
    }

    std::vector<ChkSum> &GetChksums()
    {
        return _chksums;
    }

private:
    std::mutex _mx;

    std::queue<MemoryBlock> _blocks;
    std::condition_variable _cv_block_added;
    std::condition_variable _cv_block_removed;

    size_t              _block_size;
    MemoryPool          _mempool;
    std::vector<ChkSum> _chksums;

    std::atomic<size_t> _processed_bytes;

    bool _terminate_producer;
    bool _terminate_consumer;
};

void ChksumCalculate(const char *src_fname, const char *dst_fname, size_t block_size, size_t consumers_count)
{
    File fsrc(src_fname, O_RDONLY);

    size_t blocks_count = fsrc.Size() / block_size;
    if (fsrc.Size() % block_size)
        blocks_count++;

    ChksumCalculator calculator(block_size, blocks_count);

    std::vector<std::thread> threads;
    for (size_t i = 0; i < consumers_count; i++) {
        threads.emplace_back(&ChksumCalculator::BlocksConsumer, &calculator);
    }
    threads.emplace_back(&ChksumCalculator::BlocksProducer, &calculator, fsrc.fd(), consumers_count * 2);
    for (std::thread &t : threads) {
        t.join();
    }

    if (calculator.GetProcessedBytes() != fsrc.Size())
        throw std::runtime_error("job is incomplete (file read error)");

    std::ofstream fdst(dst_fname);
    for (auto chksum : calculator.GetChksums()) {
        fdst << std::hex << chksum << ' ';
    }
}

#endif // __CHKSUM_CALCULATOR_HPP__
