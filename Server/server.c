#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <wininet.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define KEY "SUA_CHAVE_KEY"
#define PHONE_NUMBER "11988887777"
#define URL "https://api.smsdev.com.br/v1/send?key=" KEY "&type=9&number=" PHONE_NUMBER "&msg="

void sendSMS() {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char response[500];

    const char *message = "Houve um vazamento de gás";
    char url[500];
    sprintf(url, "%s%s", URL, message);

    hInternet = InternetOpenA("SMS Client", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet != NULL) {
        hConnect = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect != NULL) {
            InternetReadFile(hConnect, response, sizeof(response), &bytesRead);
            response[bytesRead] = '\0';

            printf("%s\n", response);

            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }
}


int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    char buffer[1024] = {0};
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nRequisi��o recebida com sucesso!";

    // Inicializa��o do Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Falha ao inicializar o Winsock\n");
        return 1;
    }

    // Cria��o do socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Falha ao criar o socket\n");
        return 1;
    }

    // Configura��o do endere�o do servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Vincula o socket a um endere�o e porta
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Falha ao vincular o socket\n");
        closesocket(serverSocket);
        return 1;
    }

    // Coloca o socket no modo de escuta
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        printf("Falha ao aguardar conex�es\n");
        closesocket(serverSocket);
        return 1;
    }

    printf("Servidor iniciado. Aguardando conex�es...\n");

    // Loop principal
    while (1) {
        // Aguarda por uma nova conex�o
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength)) == INVALID_SOCKET) {
            printf("Erro ao aceitar a conex�o\n");
            closesocket(serverSocket);
            return 1;
        }

        printf("Nova conex�o estabelecida\n");

        // L� a requisi��o do cliente
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
            printf("Erro ao receber a requisi��o\n");
            closesocket(clientSocket);
            continue;
        }

        printf("Requisi��o: %s\n", buffer);

        // Envia a resposta ao cliente
        if (send(clientSocket, response, strlen(response), 0) == SOCKET_ERROR) {
            printf("Erro ao enviar a resposta\n");
        } else {
            sendSMS();
            printf("Resposta enviada\n");
        }

        // Fecha o socket da conex�o
        closesocket(clientSocket);
    }

    // Encerramento do Winsock
    WSACleanup();

    return 0;
}
