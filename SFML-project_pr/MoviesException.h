
#ifndef MOVIELIBRARY_MOVIESEXCEPTION_H
#define MOVIELIBRARY_MOVIESEXCEPTION_H

#include <exception>

class bad_addr_exception : virtual public std::exception{
    const char * what() const noexcept override {
        return "Trying to reach non-existent element";
    }
};

class res_file_not_loaded : virtual public std::exception{
    const char * what() const noexcept override {
        return "Resource file is not available";
    }
};

#endif //MOVIELIBRARY_MOVIESEXCEPTION_H
