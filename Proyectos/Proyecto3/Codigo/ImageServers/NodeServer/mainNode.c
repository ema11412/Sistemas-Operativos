#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// Variables declaration in order to get the system time
struct tm *tm;

long execCounter = 0;

int callback_options(const struct _u_request *request, struct _u_response *response, void *user_data) {
    u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
    u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    u_map_put(response->map_header, "Access-Control-Allow-Headers",
              "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
    u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
    return U_CALLBACK_COMPLETE;
}

char *itoa(long n) {
    int len = n == 0 ? 1 : floor(log10l(labs(n))) + 1;
    if (n < 0) len++; // room for negative sign '-'

    char *buf = calloc(sizeof(char), len + 1); // +1 for null
    snprintf(buf, len + 1, "%ld", n);
    return buf;
}

int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *jsonImage = ulfius_get_json_body_request(request, NULL);

    if (jsonImage != NULL) {
        printf("Image received\n");

        // Get image content
        json_auto_t *imgB64 = NULL;
        imgB64 = json_object_get(jsonImage, "link");
        const char *link = json_string_value(imgB64);

        // Get image name
        json_auto_t *imgName = NULL;
        imgName = json_object_get(jsonImage, "name");
        const char *photo = json_string_value(imgName);

        // Get key
        json_auto_t *cypher = NULL;
        cypher = json_object_get(jsonImage, "xor");
        const char *xorkey = json_string_value(cypher);

        char *command;
        char *dlcmd;
        char *strCounter;

        strCounter = itoa(execCounter);
        strcat(strCounter, photo);

        dlcmd = msprintf("curl -s -X GET --output in/%s  %s", strCounter, link);
        system(dlcmd);

        int keyxor = atoi(xorkey);

        sleep(3);

        command = msprintf("./bin/p_image in/%s out/%s %d", strCounter, strCounter, keyxor);

        system(command);

        sleep(2);

        execCounter = execCounter + 1;

        json_decref(jsonImage);
        json_decref(imgB64);
        json_decref(imgName);
        json_decref(cypher);

    } else {
        printf("Error in the image received\n");
    }

    ulfius_set_string_body_response(response, 200, "Exec Done");
    return U_CALLBACK_CONTINUE;
}


int main(int argc, char **argv) {

    struct _u_instance instance;

    int port = 2020;

    // Initialize instance with the port number
    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return EXIT_FAILURE;
    }

    // callback functions declarations
    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &callback_options, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/data", NULL, 0, &receiveImage, NULL);

    // Allow all origins
    u_map_put(instance.default_headers, "Access-Control-Allow-Origin", "*");
    u_map_put(instance.default_headers, "Access-Control-Allow-Credentials", "true");
    u_map_put(instance.default_headers, "Cache-Control", "no-store");
    u_map_put(instance.default_headers, "Pragma", "no-cache");

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);
        // Wait for the user to press <enter> on the console to quit the application
        while (1) {
            sleep(100);
        }
    } else {
        fprintf(stderr, "Error starting framework\n");
    }
    printf("End framework\n");

    // Stops the framework
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return EXIT_SUCCESS;
}