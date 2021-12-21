#ifndef __MEMORY_POOL_HPP__
#define __MEMORY_POOL_HPP__

#include <memory>
#include <vector>
#include <mutex>

class MemoryPool
{
public:
    MemoryPool(size_t chunk_size)
        : _chunk_size(chunk_size)
    {
    }

    std::unique_ptr<char[]> Get()
    {
        std::unique_lock<std::mutex> lock(_mx);

        if (_pool.empty()) {
            lock.unlock();

            return std::make_unique<char[]>(_chunk_size);
        }

        auto chunk = std::move(_pool.back());
        _pool.pop_back();

        return chunk;
    }

    void Put(std::unique_ptr<char[]> &&chunk)
    {
        std::unique_lock<std::mutex> lock(_mx);
        _pool.push_back(std::move(chunk));
    }

    size_t Size()
    {
    	return _pool.size();
    }

private:
    std::vector<std::unique_ptr<char[]>> _pool;

    std::mutex _mx;
    size_t     _chunk_size;
};

class MemoryPoolTest
{
public:
	MemoryPoolTest()
		: _pool(128)
	{
	}

	bool Run()
	{
		auto p1 = _pool.Get();
		auto p2 = _pool.Get();

		auto ptr1 = p1.get();
		auto ptr2 = p2.get();

		if (_pool.Size() != 0)
			return false;

		_pool.Put(std::move(p2));
		_pool.Put(std::move(p1));

		if (_pool.Size() != 2)
			return false;

		auto p1_ = _pool.Get();
		auto p2_ = _pool.Get();

		if (_pool.Size() != 0)
			return false;

		if (ptr1 != p1_.get() ||
			ptr2 != p2_.get()) {
			return false;
		}

		return true;
	}

private:
	MemoryPool _pool;
};

#endif // __MEMORY_POOL_HPP__
