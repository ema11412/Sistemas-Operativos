#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <time.h>

// Variables declaration in order to get the system time
struct tm *tm;

void logFileCreation(char *mensaje) {
    FILE *fptr;
    fptr = fopen("/home/raul/Downloads/P3SO/VM_Services-main/ImageServer/cmake-build-debug/server.txt", "a+");
    fprintf(fptr, "Mensaje: %s\n", mensaje);
    fclose(fptr);
};


void smartManager(const char *name, const char *link, const char *xor, const char *times) {

    char *command;

    int port1 = 2020;
    int port2 = 3030;
    char *ip1 = "localhost";
    char *ip2 = "10.0.0.141";

    int x = atoi(times);


    while (x > 0) {
        char *cInfo;
        cInfo = msprintf("Quedan en la cola  %i Imagenes", x);
        logFileCreation(cInfo);
        char *ms1 = "Enviando imagen al nodo 1";
        char *ms2 = "Enviando imagen al nodo 2";

        if (x == 1) {
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            break;
        }
        if (x == 2) {

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);


            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);
            break;
        }
        if (x == 3) {

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);

            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            break;
        }
        if (x == 4) {
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);

            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);

            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);
            break;
        }
        if (x == 5) {
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);

            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);

            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);

            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            break;
        } else {
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);
            logFileCreation(ms1);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip1, port1);
            system(command);
            logFileCreation(ms2);
            command = msprintf(
                    "curl -d '{\"name\":\"%s\", \"link\":\"%s\", \"xor\":\"%s\"}' -H 'Content-Type: application/json' -X POST http://%s:%d/data",
                    name, link, xor, ip2, port2);
            system(command);
            x = x - 6;
            sleep(2);
        }
    }
}

int callback_options(const struct _u_request *request, struct _u_response *response, void *user_data) {
    u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
    u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    u_map_put(response->map_header, "Access-Control-Allow-Headers",
              "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
    u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
    return U_CALLBACK_COMPLETE;
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


        json_auto_t *times = NULL;
        times = json_object_get(jsonImage, "times");
        const char *timesx = json_string_value(times);

        smartManager(photo, link, xorkey, timesx);

        json_decref(jsonImage);
        json_decref(imgB64);
        json_decref(imgName);
        json_decref(cypher);
        json_decref(times);

    } else {
        printf("Error in the image received\n");
    }

    json_t *json_response = json_object();

    printf("POST END\n");
    json_object_set_new(json_response, "message", json_string("Correcto"));
    ulfius_set_json_body_response(response, 200, json_response);
    json_decref(json_response);
    return U_CALLBACK_CONTINUE;
}

int main(int argc, char **argv) {


    struct _u_instance instance;
    char *rest;

    int port = 1717;
    // Initialize instance with the port number
    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return EXIT_FAILURE;
    }

    // callback functions declarations
    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &callback_options, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/contraste", NULL, 0, &receiveImage, NULL);


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
