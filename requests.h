#ifndef _REQUESTS_
#define _REQUESTS_

// returneaza mesajul pentru un POST cu scopul realizarii comenzii de "login"
char* compute_post_register_login(char *host, char *url, char* content_type, char *body_data);

// returneaza mesajul pentru un POST cu scopul obtinerii accesului in biblioteca
char *compute_get_enter_library(char *host, char *url, char *cookie);

// returneaza mesajul pentru un GET cu scopul obtinerii informatiilor despre carti.
// In cazul in care in url este concatenat si un parametru, avand insemnatatea id-ului
// atunci se va returna un mesaj care face GET cu scopul obtinerii informatiei
// despre cartea a carei id a fost introdus
char *compute_get_get_books(char *host, char *url, 
                            char *header_content);                          

// returneaza un mesaj pentru POST cu scopul adaugarii unei noi carti
char *compute_post_add_book(char *host, char *url, char* content_type, char *body_data,
                            char *header_content);

// returneaza un mesaj pentru DELETE cu scopul stergerii unei carti
char *compute_delete_book(char *host, char *url, char *query_params,
                            char *header_content);

// returneaza un mesaj pentru GET cu scopul realizarii logout-ului
char *compute_get_logout(char *host, char *url, 
                             char *header_content);

#endif
