#ifndef CLIENT_H
#define CLIENT_H

void client_join(const char* hostname, int port);
void client_update();
void client_leave();

int client_getid();
void client_send(const char* data);

#endif
