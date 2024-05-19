#include "exception_handler.h"

Exception_handler::Exception_handler() {}

void Exception_handler::set_error(std::string error) {
    this->error_occured = true;
    this->error_msg = error;
}

void Exception_handler::reset_error() {
    this->error_occured = false;
    this->error_msg = "";
}
