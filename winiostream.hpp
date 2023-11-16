/**
 * Wraps around HANDLE or FILE* as streambuf-ers to
 * make ostream or istream.
 **/

#ifndef WINIOSTREAM_HPP
#define WINIOSTREAM_HPP

#include <windows.h>
#include <fcntl.h>
#include <streambuf>

using namespace std;

class ibuf : public streambuf {
private:
    FILE *file;
    char buf;

public:
    ibuf(HANDLE h) : ibuf(_open_osfhandle(INT_PTR(h), O_RDONLY|O_TEXT)) {}
    ibuf(int fd) : ibuf(_fdopen(fd, "r")) {}
    ibuf(FILE *file) : file(file) {}
    ~ibuf() {
        sync();
        if (file)
          fclose(file);
    }
    int underflow() {
        if (gptr() == egptr() && file && !feof(file)) {
            size_t size = fread(&buf, 1, 1 /* bufsize */, file);
            setg(&buf, &buf, &buf + size);
        }
        return (gptr() == egptr() || feof(file))
               ? traits_type::eof()
               : traits_type::to_int_type(*gptr());
    }
};


class obuf : public streambuf {
private:
    FILE *file;
    enum { bufsize = 512 };
    char buf[bufsize];

public:
    obuf(HANDLE h) : obuf(_open_osfhandle(INT_PTR(h), O_WRONLY|O_TEXT)) {}
    obuf(int fd) : obuf(_fdopen(fd, "w")) {}
    obuf(FILE *file) : file(file) {
      setp(buf, buf + (file? bufsize - 1: 0));
    }
    ~obuf() {
        sync();
        if (file)
            fclose(file);
    }

    int overflow(int c) {
        if (c != std::char_traits<char>::eof()) {
            *this->pptr() = std::char_traits<char>::to_char_type(c);
            pbump(1);
        }
        return sync()
                ? std::char_traits<char>::eof()
                : std::char_traits<char>::not_eof(c);
    }

    int sync() {
        std::streamsize size(pptr() - pbase());
        std::streamsize done(file? fwrite(pbase(), 1, size, file): 0);
        setp(pbase(), epptr());
        return size == done? 0: -1;
    }

    void flush() {
        if (file)
            fflush(file);
    }
};

#endif // WINIOSTREAM_HPP
