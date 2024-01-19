//
// Created by Ben on 05/01/2024.
//

#ifndef COMMAND_H
#define COMMAND_H
#include <vector>

class Command {
public:
    char* type;
    std::vector<char*> body;

    Command(char* type, std::vector<char*> body);

    Command();
};

#endif //COMMAND_H
