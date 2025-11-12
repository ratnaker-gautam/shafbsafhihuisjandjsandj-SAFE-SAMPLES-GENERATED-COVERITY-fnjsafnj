//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t, const char*);

static state_t current_state = STATE_IDLE;
static char buffer[256];
static size_t buffer_pos = 0;

state_t handle_idle(event_t event, const char* data) {
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

state_t handle_ready(event_t event, const char* data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && strlen(data) > 0) {
                size_t data_len = strlen(data);
                if (buffer_pos + data_len < sizeof(buffer)) {
                    strncpy(buffer + buffer_pos, data, sizeof(buffer) - buffer_pos - 1);
                    buffer_pos += data_len;
                    buffer[buffer_pos] = '\0';
                    printf("Data stored: %s\n", data);
                } else {
                    printf("Buffer full\n");
                    return STATE_ERROR;
                }
            }
            return STATE_READY;
        case EVENT_PROCESS:
            if (buffer_pos > 0) {
                printf("State: READY -> PROCESSING\n");
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READY;
            }
        case EVENT_RESET:
            buffer_pos = 0;
            buffer[0] = '\0';
            printf("State: READY -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

state_t handle_processing(event_t event, const char* data) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Processing successful: %s\n", buffer);
            printf("State: PROCESSING -> COMPLETE\n");
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

state_t handle_complete(event_t event, const char* data) {
    switch (event) {
        case EVENT_RESET:
            buffer_pos = 0;
            buffer[0] = '\0';
            printf("State: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event, const char* data) {
    switch (event) {
        case EVENT_RESET:
            buffer_pos = 0;
            buffer[0] = '\0';
            printf("State: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

static state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void process_event(event_t event, const char* data) {
    if (current_state < STATE_COUNT && event < EVENT_COUNT) {
        state_t new_state = state_handlers[current_state](event, data);
        if (new_state < STATE_COUNT) {
            current_state = new_state;
        } else {
            printf("Invalid state transition\n");
        }
    } else {
        printf("Invalid state or event\n");
    }
}

int main() {
    char input[256];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 6) {
            break;
        }
        
        char data[256] = "";
        if (choice == 1) {
            printf("Enter data: ");
            if (fgets(data, sizeof(data), stdin) != NULL) {
                size_t len = strlen(data);
                if (len > 0 && data[len-1] == '\n') {