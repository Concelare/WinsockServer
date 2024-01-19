#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>

#include "BlogPost.h"
#include "Command.h"
#include "ServerExeception.h"
#include "User.h"

DWORD WINAPI HandleSocket(LPVOID param) {
    // Casts the param to the socket
    const auto socket = reinterpret_cast<SOCKET>(param);

    std::cout << "[INFO] Connected to Socket Successfully \n";

    // Initialises the user pointer
    const User* user = nullptr;

    // Loops While Socket is Connected
    while (true) {
        // Initialises the Command Receive Buffer
        Command receiveBuffer{};
        // Recieve Information From Client & Cast Data to Command
        recv(socket, reinterpret_cast<char *>(&receiveBuffer), sizeof(Command), 0);

        // Switch based on the command type
        switch (receiveBuffer.type) {
            case "PostBlog":
                // Checks if user is logged in on this socket connection if not just break
                if (user == nullptr) {
                    break;
                }

                // Constructs the blog post from the command body
                const BlogPost post = BlogPost(receiveBuffer.body.at(0), receiveBuffer.body.at(1), const_cast<char*>(user->Get_Username()));
                // Adds the post to the posts vector
                BlogPost::CreatePost(post);
                // Sends Response to Client
                send(socket, static_cast<char *>(true), sizeof(true), 0);
            break;
            case "DeleteBlog":
                // Checks if user is logged in on this socket connection if not just break
                if (user == nullptr) {
                    break;
                }
                // Attempts to delete post using first value in the body parameter
                BlogPost::DeletePost(receiveBuffer.body.at(0));
                // Sends Response to client
                send(socket, static_cast<char *>(true), sizeof(true), 0);
            break;
            case "ReadBlog":
                // Checks if user is logged in on this socket connection if not just break
                if (user == nullptr) {
                    break;
                }
                // Sends posts to client
                BlogPost::GetPosts(socket);
            break;
            case "Login":
                try {
                    // Attempts to log in user, will throw error if fails
                    User user_val = User::Login(receiveBuffer.body.at(0), receiveBuffer.body.at(1));
                    // Get pointer from return value & set user to it
                    user = &user_val;
                    // Response Message
                    const char* msg = "Successfully Logged In";
                    // Send Message Response
                    send(socket, msg, strlen(msg), 0);
                }
                catch (ServerExeception &exeception) {
                    // Response Message
                    const char* msg = "Login Failed";
                    // Send Mesage Response
                    send(socket, msg, strlen(msg), 0);
                }
            break;
            case "Register":
                try {
                    // Attempts to log in user, will throw error if fails
                    User user_val = User::Register(receiveBuffer.body.at(0), receiveBuffer.body.at(1));
                    user = &user_val;
                }
                catch (ServerExeception &exeception) {
                    // Response Message
                    const char* msg = "Register Failed";
                    // Send Mesage Response
                    send(socket, msg, strlen(msg), 0);
                }
            break;
            default:
                break;
        }
    }

    return 0;
}

// Declare configuration parameters as constants
constexpr auto SERVER_PORT = 8080;
constexpr auto SERVER_ADDR = "127.0.0.1";

int main() {
    std::cout << "[INFO] Loading Server Initiated" << std::endl;
    // Initialise wsaData & Set Version
    WSADATA wsaData;
    constexpr WORD wVersionRequested = MAKEWORD(2, 2);

    // Tries to start Winsock.dll
    if (const int result = WSAStartup(wVersionRequested, &wsaData); result != 0) {
        // If it fails to start then it closes the program
        std::cerr << "[ERROR] WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    // Initialise the serverSocket
    auto serverSocket  = INVALID_SOCKET;

    // Server Socket Being Setup
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check thats the Server Socket Was Setup Correctly
    if (serverSocket == INVALID_SOCKET) {
        // If it fails Winsock cleans up the closes the program
        std::cerr << "[ERROR] Unable to create server socket. WSAGetLastError: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Initialises up the Address service
    sockaddr_in service{};

    // Setups the address service
    service.sin_family = AF_INET;
    InetPton(AF_INET, SERVER_ADDR, &service.sin_addr.s_addr);
    service.sin_port = htons(SERVER_PORT);

    // Bind the port so no one else can use it
    if (bind(serverSocket, reinterpret_cast<SOCKADDR *>(&service), sizeof(service)) == SOCKET_ERROR) {
        // If it fails winsock cleans up, the socket is disconnected and the program closes
        std::cerr << "[ERROR] Unable to bind. WSAGetLastError: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[INFO] Server Successfully Loaded" << std::endl;
    // Vector for handling threads
    std::vector<HANDLE> hdl;
    // Thread ID initialised
    DWORD threadId;

    // Server Listen on the specified socket
    if ( listen(serverSocket, 1) == SOCKET_ERROR) {
        // If it fails winsock cleans up, the socket is disconnected and the program closes
        std::cerr << "[ERROR] Unable to listen on server socket. WSAGetLastError: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Loops until program closes
    while (true) {
        // Accept Connection & sets it to acceptSocket
        const SOCKET acceptSocket = accept(serverSocket, nullptr, nullptr);

        // Checks if the acceptSocket is invalid
        if (acceptSocket == INVALID_SOCKET) {
            // If the connection failed to connect then the winsock client is cleaned up and program exited
            std::cerr << "[ERROR] accept failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }
        // Creates new thread & inserts the thread into the vector for handling threads
        hdl.insert(hdl.begin() + hdl.size(), CreateThread(nullptr,0,HandleSocket,reinterpret_cast<LPVOID>(acceptSocket),0,&threadId));
    }
}


