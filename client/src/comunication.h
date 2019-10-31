#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void client_send_message(int client_socket, int pkg_id, char * message);
int client_receive_id(int client_socket);
char * client_receive_payload(int client_socket);

/** Retorna el verdadero largo del payload*/
int real_size_payload(int pkg_id, char* message);

/** Retorna el verdadero largo del long string de cartas*/
int real_size_long_string(char* long_string);