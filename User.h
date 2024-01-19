//
// Created by Ben on 13/01/2024.
//

#ifndef USER_H
#define USER_H
#include <map>


class User {
public:
    static User Login(char* username, const char* password);

    static User Register(char* username, char* password);

    [[nodiscard]] const char* Get_Username() const;

    User(const char* username)

private:
    const char* username = nullptr;

    // Holds All Users
    static std::map<char*, char*> Users;

};



#endif //USER_H
