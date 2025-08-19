#include "server.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define ENET_IMPLEMENTATION
#include <enet/enet.h>

typedef struct {
	int x, y, z, id;
} Diff;
Diff* diff;
int diffs;

ENetAddress address;
ENetHost* host;

void server_start(int max_clients) {
	diffs = 0;

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
				char packet_data[64];
				for (int i = 0; i < diffs; i++) {
					sprintf(packet_data, "set %d %d %d %d", diff[i].x, diff[i].y, diff[i].z, diff[i].id);
					ENetPacket* packet = enet_packet_create(packet_data, strlen(packet_data), ENET_PACKET_FLAG_RELIABLE);
					if (enet_peer_send(event.peer, 0, packet) < 0)
						enet_packet_destroy(packet);
				}
				for (int i = 0; i < host->peerCount; i++) {
					sprintf(packet_data, "new %d", host->peers[i].connectID);
					ENetPacket* packet = enet_packet_create(packet_data, strlen(packet_data), ENET_PACKET_FLAG_RELIABLE);
					if (enet_peer_send(event.peer, 0, packet) < 0)
						enet_packet_destroy(packet);
				}
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Client disconnected\n");
				break;
			case ENET_EVENT_TYPE_RECEIVE:;
				int ix, iy, iz;
				float fx, fy, fz;
				int id;
				if (sscanf((char*)event.packet->data, "pos %f %f %f %d", &fx, &fy, &fz, &id) == 4) {
					// printf("update pos %f %f %f for %d\n", fx, fy, fz, id);
				}
				else if (sscanf((char*)event.packet->data, "set %d %d %d %d", &ix, &iy, &iz, &id) == 4) {
					printf("set block %d %d %d with %d\n", ix, iy, iz, id);
					diff = realloc(diff, sizeof(Diff) * (diffs + 1));
					diff[diffs] = (Diff){ix, iy, iz, id};
					diffs++;
				}

				enet_host_broadcast(host, 0, event.packet);
				break;
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

void server_stop() {
	free(diff);
	enet_host_destroy(host);
}

int main(int argc, char* argv[]) {
	int max_clients = 2;

	int opt = 0;
	while ((opt = getopt(argc, argv, "c:") != -1)) {
		switch (opt) {
			case 'c':
				if (atoi(optarg) > 0)
					max_clients = atoi(optarg);
				break;
		}
	}

	server_start(max_clients);
	while (1) server_update();
	server_stop();
	return 0;
}
