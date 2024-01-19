//
// Created by Ben on 05/01/2024.
//

#include "Command.h"

#include <utility>

// Usage of Constructor For Command Class
Command::Command(char* type, std::vector<char*> body) {
    this->type = type;
    this->body = std::move(body);

}

// Default Constructor for Command Class
Command::Command() = default;

