//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
size_t buffer_pos = 0;

void handle_idle(Event event, const char* data) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            current_state = STATE_READY;
            buffer_pos = 0;
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(Event event, const char* data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && buffer_pos < sizeof(buffer) - 1) {
                size_t data_len = strlen(data);
                if (buffer_pos + data_len < sizeof(buffer) - 1) {
                    strcpy(buffer + buffer_pos, data);
                    buffer_pos += data_len;
                    printf("Data added: %s\n", data);
                } else {
                    printf("Buffer full, cannot add data\n");
                }
            }
            break;
        case EVENT_PROCESS:
            if (buffer_pos > 0) {
                printf("Transition: READY -> PROCESSING\n");
                current_state = STATE_PROCESSING;
            } else {
                printf("No data to process\n");
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            current_state = STATE_IDLE;
            buffer_pos = 0;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(Event event, const char* data) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
            current_state = STATE_ERROR;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(Event event, const char* data) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            current_state = STATE_IDLE;
            buffer_pos = 0;
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(Event event, const char* data) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            current_state = STATE_IDLE;
            buffer_pos = 0;
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

Event get_event_from_input(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return EVENT_FAILURE;
}

int main() {
    char input[100];
    char data_input[100];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, success, failure, reset, quit\n");
    
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
        }
        
        if (strcmp(input, "data") == 0) {
            printf("Enter data: ");
            if (fgets(data_input, sizeof(data_input), stdin) != NULL)