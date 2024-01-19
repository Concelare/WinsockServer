//
// Created by Ben on 12/01/2024.
//

#include "ServerExeception.h"

ServerExeception::ServerExeception(char* description, const int code) {
    this->description = description;
    this->code = code;
}

const char* ServerExeception::what() const noexcept {
    return this->description;
}
