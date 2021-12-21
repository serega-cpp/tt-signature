#ifndef __CHKSUM_HPP__
#define __CHKSUM_HPP__

#include <boost/crc.hpp>

typedef int32_t ChkSum;

ChkSum CalcChkSum(const char *buf, size_t size)
{
    boost::crc_32_type result;
    result.process_bytes(buf, size);
    return result.checksum();
}

#endif // __CHKSUM_HPP__
