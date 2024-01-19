//
// Created by Ben on 12/01/2024.
//

#ifndef SERVEREXECEPTION_H
#define SERVEREXECEPTION_H
#include <exception>

// Inheriting From std::exception
class ServerExeception final : public std::exception {
public:
    char* description;
    int code;

    // Constructor desc
    ServerExeception(char* description, int code);

    // Polymorphism
    [[nodiscard]] const char* what() const noexcept override;
};



#endif //SERVEREXECEPTION_H
