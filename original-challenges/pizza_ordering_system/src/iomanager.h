#ifndef IOMANAGER_H_
#define IOMANAGER_H_

extern "C" {
#include <stdlib.h>
};

class cgc_IoManager
{
private:
    static const int MAX_LINE = 512;
    int fd;
    char *line;
    cgc_size_t line_size;
    cgc_size_t line_len;
public:
    cgc_IoManager(cgc_size_t _line_size=MAX_LINE);
    virtual ~cgc_IoManager();

    bool cgc_readline(cgc_size_t max_size=0);
    int cgc_readnum(cgc_size_t max_size=0);
    void cgc_set_fd(int _fd) { fd = _fd; }
    char *cgc_get_last_input() { return line_len ? line : NULL; }
};

#endif
