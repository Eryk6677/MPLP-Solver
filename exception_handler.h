#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

#include <string>



class Exception_handler {
public:
    Exception_handler();

    bool error_occured;
    std::string error_msg;

    void set_error(std::string error);
    void reset_error();
};

#endif // EXCEPTION_HANDLER_H
