/*
 * Copyright 2015 Bagrat Ter-Akopyan
 */

#ifndef WIKIDB_RECORD_NOT_FOUND_H_
#define WIKIDB_RECORD_NOT_FOUND_H_

#include <exception>
#include <string>

class record_not_found : public std::exception {
 public:
    explicit record_not_found(std::string const& reason): _reason(reason) {}
    virtual const char* what() const noexcept {
        return _reason.c_str();
    };
 private:
    std::string _reason;
};

#endif  // WIKIDB_RECORD_NOT_FOUND_H_
