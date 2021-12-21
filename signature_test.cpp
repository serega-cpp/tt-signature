#include <cstdio>
#include <iostream>

#include "memory_pool.hpp"
#include "chksum_calculator.hpp"

static const char *cTestSource = "testfile.txt";
static const char *cTestDestination = "testfile.tmp";

int main(int argc, char *argv[])
{
	try {
		std::cerr << "MemoryPool testing... ";
		MemoryPoolTest mp_test;
		if (!mp_test.Run()) {
			std::cerr << "FAILED" << std::endl;
			return -1;
		}
		std::cerr << "OK" << std::endl;

		std::cerr << "ChksumCalculator testing... ";
		ChksumCalculate(cTestSource, cTestDestination, 400, std::thread::hardware_concurrency());

		std::string chksums;
		std::ifstream f(cTestDestination);
		std::getline(f, chksums);

		if (chksums.compare("e276e79c 58462660 602c5579 ") != 0) {
			std::cerr << "FAILED" << std::endl;
			return -1;			
		}
		std::cerr << "OK" << std::endl;

		std::remove(cTestDestination);
	}
	catch(std::exception &ex) {
		std::cerr << std::endl << "Exception: " << ex.what() << std::endl;
		std::cerr << "Test FAILED" << std::endl;
		return -1;
	}

	return 0;
}
