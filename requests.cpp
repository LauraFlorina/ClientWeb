#include <iostream>
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

using namespace std;

char* compute_post_register_login(char *host, char *url, char* content_type, char *body_data) {
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // add necessary headers (Content-Type and Content-Length are mandatory)
    
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);

    // add new line at end of header
    compute_message(message, "");

    // add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}

char *compute_get_enter_library(char *host, char *url, char *cookie)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *cookie_buffer = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add the cookie
    if (cookie != NULL) {
       strcpy(cookie_buffer, "Cookie: ");
       strcat(cookie_buffer, cookie);
       strcat(cookie_buffer, "\n");    
       compute_message(message, cookie_buffer);
    }
    
    // add final new line
    compute_message(message, "");
    return message;
}

char *compute_get_get_books(char *host, char *url, 
                             char *header_content) {
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *header_buffer = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add headers
    if (header_content != NULL) {
        strcpy(header_buffer, "Authorization");
        strcat(header_buffer, ": Bearer ");
        strcat(header_buffer, header_content); 
        compute_message(message, header_buffer);
    }
    
    // add final new line
    compute_message(message, "");
    return message;
}


char *compute_post_add_book(char *host, char *url, char* content_type, char *body_data,
                            char *header_content)
{
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *header_buffer = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add necessary headers
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);

    if (header_content != NULL) {
        strcpy(header_buffer, "Authorization");
        strcat(header_buffer, ": Bearer ");
        strcat(header_buffer, header_content); 
        compute_message(message, header_buffer);
    }

    // add new line at end of header
    compute_message(message, "");

    // add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, body_data);

    free(line);
    return message;
}

char *compute_delete_book(char *host, char *url, char *query_params,
                            char *header_content)
{   
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *header_buffer = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL
    if (query_params != NULL) {
        sprintf(line, "DELETE %s%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add headers
    if (header_content != NULL) {
        strcpy(header_buffer, "Authorization");
        strcat(header_buffer, ": Bearer ");
        strcat(header_buffer, header_content); 
        compute_message(message, header_buffer);
    }
    
    // add final new line
    compute_message(message, "");
    return message;
}

char *compute_get_logout(char *host, char *url, 
                             char *cookie) {
    char *message = (char*)calloc(BUFLEN, sizeof(char));
    char *line = (char*)calloc(LINELEN, sizeof(char));
    char *cookie_buffer = (char*)calloc(LINELEN, sizeof(char));

    // write the method name, URL
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add the cookie
    if (cookie != NULL) {
       strcpy(cookie_buffer, "Cookie: ");
       strcat(cookie_buffer, cookie);
       strcat(cookie_buffer, "\n");    
       compute_message(message, cookie_buffer);
    }
    
    // add final new line
    compute_message(message, "");
    return message;
}