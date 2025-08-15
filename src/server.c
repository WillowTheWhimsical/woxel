#include "server.h"

#include <stdio.h>
#define ENET_IMPLEMENTATION
#include <enet/enet.h>

ENetAddress address;
ENetHost* host;

void server_start(int max_clients) {
	enet_initialize();

	address = (ENetAddress){ENET_HOST_ANY, PORT};
	host = enet_host_create(&address, max_clients, 1, 0, 0);

	printf("Started server on port %d with %d max clients\n", PORT, max_clients);
}

void server_update() {
	ENetEvent event;
	while (enet_host_service(host, &event, 1000) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("Client connected %d\n", event.peer->connectID);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Client disconnected %d\n", event.peer->connectID);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("Event `%s` from client %d\n", event.packet->data, event.peer->connectID);

				float x, y, z;
				int id;
				if (sscanf((char*)event.packet->data, "pos %f %f %f %d", &x, &y, &z, &id) == 4) {
					printf("update pos %f %f %f for %d\n", x, y, z, id);
				}
				else if (sscanf((char*)event.packet->data, "set %f %f %f %d", &x, &y, &z, &id) == 4) {
					printf("set block %d %d %d with %d\n", (int)x, (int)y, (int)z, id);
				}

				enet_host_broadcast(host, 0, event.packet);
				break;
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

void server_stop() {
	enet_host_destroy(host);
}

int main(int argc, char* argv[]) {
	server_start(2);
	while (1) server_update();
	server_stop();
	return 0;
}
