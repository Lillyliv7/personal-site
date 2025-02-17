#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libwebsockets.h>

#include "cvector.h"

#define PORT 3000
volatile unsigned int connections_number = 0;
volatile cvector_vector_type(char*)* list;

bool verifyUser(const char* ip) {
    printf("WSSERVER: checking user %s\n", ip);
    for(unsigned long long i = 0; i < cvector_size(*list); i++) {
        if(!strcmp(ip, *list[i])) {
            printf("WSSERVER: user %s is not unique\n", ip);
            connections_number++;
            return false;
        }
    }

    printf("WSSERVER: user %s is unique\n", ip);
    cvector_push_back(*list, ip);
    return true;
}

static int statsCallback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("WSSERVER: Connection established\n");
            char* user_ip = malloc(20);
            lws_get_peer_simple(wsi, user_ip, 19);
            verifyUser(user_ip);
            break;
        case LWS_CALLBACK_RECEIVE: {
            const char *message = "unique visitors: X"; // build your message accordingly
            size_t message_len = strlen(message);
            unsigned char buf[LWS_PRE + message_len];
            memcpy(&buf[LWS_PRE], message, message_len);
            lws_write(wsi, &buf[LWS_PRE], message_len, LWS_WRITE_TEXT);
            break;
        }
        //     // Send the client the total number of unique visitors so far
        //     lws_write(wsi, in, 8, connections_number);
        //     break;
        
        case LWS_CALLBACK_CLOSED:
            printf("WSSERVER: Connection closed\n");
            break;
        default:
            break;
    }
    return 0;
}

// Define the WebSocket protocol
static struct lws_protocols protocols[] = {
    { "stats-protocol", statsCallback, 0, 0 },
    { NULL, NULL, 0, 0 } // End of protocols
};

int main(int argc, char **argv) {
    // create cvector of visitors in the past
    cvector_init(*list, 500, NULL);



    // WebSocket server context setup
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = PORT;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    // Create the WebSocket context
    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Failed to create WebSocket context\n");
        return 1;
    }

    printf("WebSocket server started on port %d\n", PORT);

    // Main event loop
    while (1) {
        lws_service(context, 50); // Poll for events every 50 ms
    }

    lws_destroy_context(context);
    return 0;
}