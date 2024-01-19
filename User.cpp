//
// Created by Ben on 13/01/2024.
//

#include "User.h"

#include <cstring>

#include "ServerExeception.h"

User User::Login(char* username, const char* password) {
    // Attempts to Retrieve Save Password
    const char* saved_password = Users.at(username);

    // Checks if save password is null
    if (saved_password == nullptr) {
        // Throw Error That User Doesn't Exist
        throw ServerExeception("No User Found!", 001);
    }

    // Check if passwords Match
    if (strcmp(password, saved_password) != 0) {
        // If passwords are incorrect throw error
        throw ServerExeception("Incorrect Password", 002);
    }

    // Returns User Object With Username Set
    return {username};
}

User User::Register(char* username, char* password) {
    // Checks if User Already Exists
    if (Users.at(username) != nullptr) {
        // If user exists throw error saying it exists
        throw ServerExeception("User Found!", 003);
    }

    // Insert User Into The User Vector
    Users.insert(username, password);

    // Returns User Object With Username Set
    return {username};
}

// Abstraction: Gets Username
const char* User::Get_Username() const {
    return this->username;
}

// Constructor for User
User::User(const char* username) {
    this->username = username;
}
