#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>

class File
{
public:
    File(const char *fname, int mode)
    {
        _fd = open(fname, mode);
        if (_fd == -1)
           throw std::runtime_error("input file open");
    }

    ~File()
    {
        close(_fd);
    }

    size_t Size()
    {
        struct stat file_stat;

        if (fstat(_fd, &file_stat) != 0)
            throw std::runtime_error("input file stat");

        return file_stat.st_size;
    }

    int fd()
    {
        return _fd;
    }

private:
    int _fd;
};

#endif // __FILE_HPP__
