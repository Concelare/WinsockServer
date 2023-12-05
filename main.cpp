#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>

using namespace std;

DWORD WINAPI HandleSocket(LPVOID param) {
    const auto socket = reinterpret_cast<SOCKET>(param);

    cout << "[INFO] Connected to Socket Successfully \n";

    while (true) {
        char receiveBuffer[200] = "";
        const int byteCount = recv(socket, receiveBuffer, 200, 0);

        if (byteCount < 0){
            cerr << "[ERROR] Client: " <<  WSAGetLastError() << endl;
            return 1;
        }

        if (byteCount > 0) {
            cout << "[INFO] Data Recieved: " << receiveBuffer << endl;
            if (strcmp(receiveBuffer, "s") == 0) {
                cout << "[Info] Client Says: " << receiveBuffer << endl;
            }
            if (strcmp(receiveBuffer, "q") == 0) {
                cout << "[Info] Socket Closed" << endl;
                closesocket(socket);
                return 0;
            }
        }
    }

    return 0;
}

// Declare configuration parameters as constants
constexpr auto SERVER_PORT = 8080;
constexpr auto SERVER_ADDR = "127.0.0.1";

int main() {
    cout << "[INFO] Loading Server Initiated" << endl;
    WSADATA wsaData;
    constexpr WORD wVersionRequested = MAKEWORD(2, 2);

    if (const int result = WSAStartup(wVersionRequested, &wsaData); result != 0) {
        cerr << "[ERROR] WSAStartup failed with error: " << result << endl;
        return 1;
    }

    auto serverSocket  = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        cerr << "[ERROR] Unable to create server socket. WSAGetLastError: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in service{};
    service.sin_family = AF_INET;
    InetPton(AF_INET, SERVER_ADDR, &service.sin_addr.s_addr);
    service.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, reinterpret_cast<SOCKADDR *>(&service), sizeof(service)) == SOCKET_ERROR) {
        cerr << "[ERROR] Unable to bind. WSAGetLastError: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "[INFO] Server Successfully Loaded" << endl;
    vector<HANDLE> hdl;
    DWORD threadId;
    if ( listen(serverSocket, 1) == SOCKET_ERROR) {
        cerr << "[ERROR] Unable to listen on server socket. WSAGetLastError: " << WSAGetLastError() << endl;
        closesocket(serverSocket);  // Close the serverSocket on listen() error
        WSACleanup();
        return 1;
    }


    while (true) {

        const SOCKET acceptSocket = accept(serverSocket, nullptr, nullptr);

        if (acceptSocket == INVALID_SOCKET) {
            cerr << "[ERROR] accept failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            return 1;
        }

        hdl.insert(hdl.begin() + hdl.size(), CreateThread(nullptr,0,HandleSocket,reinterpret_cast<LPVOID>(acceptSocket),0,&threadId));

    }

    WSACleanup();
    return 0;
}


