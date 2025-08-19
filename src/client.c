#include "client.h"
#include "entities.h"
#include "entity.h"
#include "world.h"

#include <stdio.h>
#include <string.h>
#define ENET_IMPLEMENTATION
#include <enet/enet.h>

ENetAddress address;
ENetHost* client;
ENetPeer* peer;

void client_join(const char* hostname, int port) {
	enet_initialize();

	enet_address_set_host(&address, hostname);
	address.port = port;
	client = enet_host_create(NULL, 1, 1, 0, 0);
	peer = enet_host_connect(client, &address, 1, 0);

	printf("Joining server at %s:%d\n", hostname, port);
}

void client_update() {
	ENetEvent event;
	while (enet_host_service(client, &event, 10) > 0) {
		switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("Joined server\n");
				char packet_data[64];
				sprintf(packet_data, "new %d", event.peer->connectID);
				printf("Sending id %d\n", event.peer->connectID);
				client_send(packet_data);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Left server\n");
				break;
			case ENET_EVENT_TYPE_RECEIVE:;
				float x, y, z;
				int id;
				if (sscanf((char*)event.packet->data, "new %d", &id) == 1) {
					Entity* e = spawn_entity(E_PUPPET);
					*(unsigned int*)e->var[0] = event.peer->connectID;
					printf("Client joined server %d\n", id);
				}
				else if (sscanf((char*)event.packet->data, "pos %f %f %f %d", &x, &y, &z, &id) == 4) {
					for_entities(e)
						if (e->type == E_PUPPET && *(unsigned int*)e->var[0] == id) {
							e->pos = (Vector3){x, y, z};
							break;
						}
					}
				}
				else if (sscanf((char*)event.packet->data, "set %f %f %f %d", &x, &y, &z, &id) == 4) {
					set_block((int)x, (int)y, (int)z, id);
				}

				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

void client_leave() {
	enet_peer_reset(peer);
	enet_host_destroy(client);
}

int client_getid() {
	return peer->connectID;
}

void client_send(const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}
