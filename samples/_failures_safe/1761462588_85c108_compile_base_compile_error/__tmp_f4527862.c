//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef void (*StateHandler)(Event event, const char* data);

void handle_idle(Event event, const char* data);
void handle_ready(Event event, const char* data);
void handle_processing(Event event, const char* data);
void handle_complete(Event event, const char* data);
void handle_error(Event event, const char* data);

StateHandler state_handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

State current_state = STATE_IDLE;
char buffer[256];
int data_length = 0;

void handle_idle(Event event, const char* data) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(Event event, const char* data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL) {
                size_t len = strlen(data);
                if (len < sizeof(buffer)) {
                    strncpy(buffer, data, sizeof(buffer) - 1);
                    buffer[sizeof(buffer) - 1] = '\0';
                    data_length = (int)len;
                    printf("Data stored: %s\n", buffer);
                    current_state = STATE_PROCESSING;
                    printf("Transition: READY -> PROCESSING\n");
                } else {
                    printf("Data too long\n");
                    current_state = STATE_ERROR;
                    printf("Transition: READY -> ERROR\n");
                }
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(Event event, const char* data) {
    switch (event) {
        case EVENT_PROCESS:
            if (data_length > 0) {
                int valid = 1;
                for (int i = 0; i < data_length; i++) {
                    if (buffer[i] < ' ' || buffer[i] > '~') {
                        valid = 0;
                        break;
                    }
                }
                if (valid) {
                    printf("Processing successful\n");
                    current_state = STATE_COMPLETE;
                    printf("Transition: PROCESSING -> COMPLETE\n");
                } else {
                    printf("Invalid characters in data\n");
                    current_state = STATE_ERROR;
                    printf("Transition: PROCESSING -> ERROR\n");
                }
            } else {
                printf("No data to process\n");
                current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR\n");
            }
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(Event event, const char* data) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Operation completed successfully\n");
            break;
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(Event event, const char* data) {
    switch (event) {
        case EVENT_FAILURE:
            printf("Operation failed\n");
            break;
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(Event event, const char* data) {
    if (current_state >= STATE_IDLE && current_state <= STATE_ERROR) {
        state_handlers[current_state](event, data);
    }
}

int main() {
    char input[512];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data <text>, process, success, failure, reset, quit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "start") == 0) {
            process_event(EVENT_START, NULL