#include "comunication.h"
int size_payload;

int client_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char * client_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  //Guardamos el tamaño para los logs
  size_payload = len;

  // Se obtiene el payload
  char * payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

void client_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = real_size_payload(pkg_id,message) + 1; //+1 para considerar el caracter nulo. 
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.
  
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}


/** Retorna el verdadero largo del payload*/
int real_size_payload(int pkg_id, char* message){
  int payloadSize;

  if(pkg_id ==6 || pkg_id == 7|| pkg_id == 12 || pkg_id ==13 || pkg_id == 14 || pkg_id == 16){
    payloadSize = 1;
  }

  else if(pkg_id == 8 || pkg_id == 11){
    payloadSize = 2;
  }

  else if(pkg_id == 9){
    payloadSize = real_size_long_string(message);
  }

  else{
    payloadSize = strlen(message);
  }

  return payloadSize;
}

/** Retorna el verdadero largo del long string de cartas*/
int real_size_long_string(char* long_string){
  int total_size = 0;
  int aux = 0;
  int count = 0;
  while(count<20){
    int largo = long_string[aux];
    total_size = total_size + largo + 2;
    count++;
    aux = aux + largo + 2;
  }
  return total_size;
}