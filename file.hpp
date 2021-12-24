#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>

#ifdef _WIN32
  #include <io.h>

  #define open     _open
  #define close    _close
  #define fstat    _fstat
  #define read     _read
  #define stat     _stat
  #define O_RDONLY (_O_RDONLY | _O_BINARY)
#else
  #include <unistd.h>
#endif

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
