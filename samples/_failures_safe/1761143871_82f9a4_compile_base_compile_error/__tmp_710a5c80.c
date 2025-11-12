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
size_t buffer_pos = 0;

void handle_idle(Event event, const char* data) {
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READY\n");
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
                if (buffer_pos + data_len < sizeof(buffer)) {
                    strcpy(buffer + buffer_pos, data);
                    buffer_pos += data_len;
                    printf("Data received: %s\n", data);
                } else {
                    printf("Buffer overflow prevented\n");
                }
            }
            break;
        case EVENT_PROCESS:
            if (buffer_pos > 0) {
                printf("State: READY -> PROCESSING\n");
                current_state = STATE_PROCESSING;
            } else {
                printf("No data to process\n");
            }
            break;
        case EVENT_RESET:
            printf("State: READY -> IDLE\n");
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
            printf("State: PROCESSING -> COMPLETE\n");
            current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            printf("State: PROCESSING -> ERROR\n");
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
            printf("State: COMPLETE -> IDLE\n");
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
            printf("State: ERROR -> IDLE\n");
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

int main() {
    char input[256];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("1: START, 2: DATA, 3: PROCESS, 4: SUCCESS, 5: FAILURE, 6: RESET, 0: EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter event number: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        Event event;
        const char* data = NULL;
        
        switch (choice) {
            case 1: event = EVENT_START; break;
            case 2: 
                event = EVENT_DATA;
                printf("Enter data: ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    input[strcspn(input, "\n")] = '\0';
                    data = input;
                }
                break;
            case 3: event = EVENT_PROCESS; break;
            case 4: event = EVENT_SUCCESS; break;
            case 5: event = EVENT_FAILURE; break;
            case 6: event = EVENT_RES