#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> //for inetpton
#include <tchar.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")


/*
    //initialize winsock library

    //create the socket

    //get ip and port

    //bind the ip/port with the socket

    //listen on the socket

    //accept

    //recv and send

    //close the socket

    //clean up the winsock

*/

bool Initialize(){
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2), &data) == 0;

}

int main(){
    if(!Initialize()){
        cout << "winsock initialization failed" << endl;

    }

    cout << "server program" << endl;

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(listenSocket == INVALID_SOCKET){
        cout << "socket creation failed" << endl;
        return 1;
    }

    //create address structure
    int port = 12345;
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    //convert the ipaddress(0.0.0.0) put it inside the sin_family in binary form
    if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1){
        cout << "setting address structure failed " << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //bind 
    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){
        cout << "bind failed" <<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //listen
    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        cout << "listen failed" <<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    cout << "server has started listening on port : " << port << endl;

    //accept 
    SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
    if(clientSocket == INVALID_SOCKET){
        cout << "invalid client socket" << endl;
    }

    char buffer[4096];
    int bytesrecvd= recv(clientSocket, buffer, sizeof(buffer), 0);

    string message(buffer, bytesrecvd);
    cout << "message from client: " << message << endl;

    closesocket(clientSocket);

    closesocket(listenSocket);


    WSACleanup();
    return 0;
}