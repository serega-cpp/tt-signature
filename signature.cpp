#include <iostream>
#include <thread>

#include "options.hpp"
#include "chksum_calculator.hpp"

int main(int argc, char *argv[])
{
    try {
        Options opt(argc, argv);

        if (opt.help) {
            std::cout << opt.Usage() << std::endl;
            return 1;
        }

        ChksumCalculate(opt.src_fname.c_str(), opt.dst_fname.c_str(), opt.block_size, std::thread::hardware_concurrency());
    }
    catch (const std::exception &ex) {
        std::cerr << std::endl << "Exception: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
