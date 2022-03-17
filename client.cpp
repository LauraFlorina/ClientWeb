#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy,as memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

#define CMD_LEN 50
#define DATA_LEN 50
#define CODE_LEN 3
#define COOKIE_LEN 150
#define ID_LEN 10

using namespace std;
using json = nlohmann::json;

// Se verifica daca eroarea s-a realizat cu succes sau a aparut
// o eroare
int analyzes_response(char *response) {
    string response_string(response);
    int index1 = response_string.find(" ");
    int index2 = response_string.find("\n");
    string returned_status = response_string.substr(index1 + 1, index2 - index1);
    cout << "\nCodul si mesajul returnat: " << returned_status;
            
    if (returned_status[0] == '2') {
        cout << "SUCCES \n";
        return 1;
    } else {
        cout << "EROARE \n";
        cout << extract_error_message(response) << "\n\n";
        return 0;
    }
}

void perform_register() {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/auth/register";
    char content_type[] = "application/json";

    // Se citesc username si parola
    char *username = (char*)calloc(DATA_LEN, sizeof(char));
    char *password = (char*)calloc(DATA_LEN, sizeof(char));
    cout << "username= ";
    cin >> username;
    cout << "password= ";
    cin >> password;
    // Se creeaza obiectul json
    json j;
    j["username"] = username;
    j["password"] = password;

    // Se face post cu scopul realizarii inregistrarii
    message = compute_post_register_login(main_server, url, content_type, (char*)j.dump().c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    analyzes_response(response);
    close_connection(sockfd);
    cout << "\n";
}

string perform_login() {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/auth/login";
    char content_type[] = "application/json";

    // Se citesc username si parola
    char *username = (char*)calloc(DATA_LEN, sizeof(char));
    char *password = (char*)calloc(DATA_LEN, sizeof(char));
    cout << "username= ";
    cin >> username;
    cout << "password= ";
    cin >> password;
    // Se creeaza obiectul json
    json j;
    j["username"] = username;
    j["password"] = password;

    // Se face post cu scopul realizarii autentificarii
    message = compute_post_register_login(main_server, url, content_type, (char*)j.dump().c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    int result = analyzes_response(response);

    // Extragere cookie
    if (result == 0) {
        return "";
    }

    string response_string(response);
    int index3 = response_string.find("Set-Cookie: ");
    int index4 = response_string.find("\n", index3);
    string cookie = response_string.substr(index3 + strlen("Set-Cookie: "),
                            index4 - index3 - strlen("Set-Cookie: "));

    cout << "\n";
    close_connection(sockfd);
    return cookie;
}

string perform_entry_library(string cookie) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/library/access";

    // Se face post cu scopul obtinerii accesului de intrare in biblioteca
    message = compute_get_enter_library(main_server, url, (char *)cookie.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    int result = analyzes_response(response);

    // Extragere token
    if (result == 0) {
        return "";
    }
    string response_string(response);
    int index3 = response_string.find("{\"token\":\"");
    int index4 = response_string.find("\"}");
    string returned_str = response_string.substr(index3 + strlen("{\"token\":\""),
                    index4 - index3 - strlen("{\"token\":\""));

    cout << "\n";
    close_connection(sockfd);
    return returned_str;
}

void perform_get_books(string token) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/library/books";

    // Se face get cu scopul obtinerii cartilor
    message = compute_get_get_books(main_server, url, (char *)token.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    int result = analyzes_response(response);

    // Se afiseaza cartile
    if (result == 0) {
        return;
    }
    
    cout << "Carti: \n";
    cout << extract_books(response) << "\n";
    cout << "\n";
    close_connection(sockfd);
}

void perform_get_book(string token) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/library/books/";
    char *id = (char *)calloc(ID_LEN, sizeof(char));

    // Se citeste id-ul
    cout << "id= ";
    cin >> id;

    strcat(url, id);

    // Se face get cu scopul obtinerii informatiilor despre o carte
    message = compute_get_get_books(main_server, url, (char *)token.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    // Se analizeaza mesajul raspunsul intors de server
    int result = analyzes_response(response);

    // Se afiseaza cartea
    if (result == 0) {
        return;
    }

    cout << "Informatii despre carte \n";
    cout << extract_books(response) << "\n";
    cout << "\n";
    close_connection(sockfd);
}


void perform_add_book(string token) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/library/books";
    char content_type[] = "application/json";

    string title;
    string author;
    string genre;
    string publisher;
    string page_count;

    getline(std::cin, title, '\n');
    cout << "title=";
    getline(std::cin, title);
    cout << "author=";
    getline(std::cin, author);
    cout << "genre=";
    getline(std::cin, genre);
    cout << "publisher=";
    getline(std::cin, publisher);
    cout << "page_count=";
    getline(std::cin, page_count);
    
    // Se creaza obiectul json
    json j;
    j["title"] = title;
    j["author"] = author;
    j["genre"] = genre;
    j["publisher"] = publisher;
    j["page_count"] = page_count;

    // Se face post cu scopul adaugarii unei noi carti
    message = compute_post_add_book(main_server, url, content_type, (char*)j.dump().c_str(),
                                    (char *)token.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    analyzes_response(response);
    close_connection(sockfd);
    cout << "\n";
}

void perform_delete_book(string token) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/library/books/";
    char *id = (char *)calloc(ID_LEN, sizeof(char));

    // Se citeste id-ul
    cout << "id= ";
    cin >> id;

    // Se face delete cu scopul stergerii unei carti
    message = compute_delete_book(main_server, url, id, (char *)token.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    analyzes_response(response);
    close_connection(sockfd);
    cout << "\n";
}

void perform_logout(string cookie) {
    char *message;
    char *response;
    int sockfd;

    char main_server[] = "34.118.48.238";
    sockfd = open_connection(main_server, 8080, AF_INET, SOCK_STREAM, 0);
    char url[] = "/api/v1/tema/auth/logout";

    message = compute_get_logout(main_server, url, (char *)cookie.c_str());
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    // Se analizeaza mesajul raspunsul intors de server
    analyzes_response(response);
    close_connection(sockfd);
    cout << "\n";
}

int main(int argc, char *argv[])
{
    char *command = (char *) calloc(CMD_LEN, sizeof(char));
    string cookie;
    string token;    
    while (1) {
        cout << "Introduceti comanda dorita: \n";
        cin >> command;
        cout << "Comanda citita este: " << command << "\n";
        
        if (strcmp(command, "register") == 0) {
            perform_register();
        } else if (strcmp(command, "login") == 0) {
            if (!cookie.empty()) {
                cookie.clear();
            }
            if (!token.empty()) {
                token.clear();
            }
            cookie = perform_login();
            if (cookie.empty()) {
                continue;
            }
        } else if (strcmp(command, "enter_library") == 0) {
            token = perform_entry_library(cookie);
            if (token.empty()) {
                continue;
            }
        } else if (strcmp(command, "get_books") == 0) {
            perform_get_books(token);
        } else if (strcmp(command, "get_book") == 0) {
            perform_get_book(token);
        } else if (strcmp(command, "add_book") == 0) {
            perform_add_book(token);
        } else if (strcmp(command, "delete_book") == 0) {
            perform_delete_book(token);
        } else if (strcmp(command, "logout") == 0) {
            perform_logout(cookie);
            if (!cookie.empty()) {
                cookie.clear();
            }
            if (!token.empty()) {
                token.clear();
            }
        } else if (strcmp(command, "exit") == 0) {
            exit(1);
        } else {
            cout << "Introduceti o comanda valida. \n";
        }
    }
    return 0;
}
