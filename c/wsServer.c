#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libwebsockets.h>
#include "cvector.h"

#define PORT 3000

volatile unsigned int connections_number = 0;
// Declare the vector as a variable (not a pointer)
volatile cvector_vector_type(char*) list = NULL;

bool verifyUser(const char* ip) {
    printf("WSSERVER: checking user %s\n", ip);
    for (unsigned long long i = 0; i < cvector_size(list); i++) {
        // strcmp returns 0 if the strings are equal
        if (strcmp(ip, list[i]) == 0) {
            printf("WSSERVER: user %s is not unique\n", ip);
            connections_number++;
            return false;
        }
    }

    printf("WSSERVER: user %s is unique\n", ip);
    // Duplicate the string so that the vector owns its own copy
    cvector_push_back(list, strdup(ip));
    return true;
}

static int statsCallback(struct lws *wsi, enum lws_callback_reasons reason,
                           void *user, void *in, size_t len) {
    switch (reason) {

        case LWS_CALLBACK_ESTABLISHED: {
            printf("WSSERVER: Connection established\n");
            char *user_ip = malloc(20);
            if (!user_ip) {
                lwsl_err("Memory allocation failed\n");
                return -1;
            }
            lws_get_peer_simple(wsi, user_ip, 19);
            verifyUser(user_ip);
            free(user_ip);
            break;
        }

        case LWS_CALLBACK_RECEIVE:
            // Instead of writing immediately, schedule a writable callback.
            lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            const char *message = "unique visitors: X";  // Adjust as needed
            size_t message_len = strlen(message);

            // Allocate a temporary buffer with LWS_PRE bytes reserved.
            unsigned char *buf = malloc(LWS_PRE + message_len);
            if (!buf) {
                lwsl_err("Out of memory\n");
                return -1;
            }
            memcpy(buf + LWS_PRE, message, message_len);

            // Write the message. Note that lws_write expects the pointer to start at LWS_PRE.
            int n = lws_write(wsi, buf + LWS_PRE, message_len, LWS_WRITE_TEXT);
            free(buf);

            if (n < (int)message_len) {
                lwsl_err("lws_write failed\n");
                return -1;
            }
            break;
        }

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
    { NULL, NULL, 0, 0 }  // End of protocols
};

int main(int argc, char **argv) {
    // Initialize the cvector
    cvector_init(list, 500, NULL);

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
