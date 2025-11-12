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

typedef state_t (*state_handler_t)(event_t);

static state_t current_state = STATE_IDLE;
static char buffer[256];
static size_t buffer_pos = 0;

state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            buffer_pos = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

state_t handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
            if (buffer_pos < sizeof(buffer) - 1) {
                buffer[buffer_pos++] = 'A';
                return STATE_READY;
            }
            return STATE_ERROR;
        case EVENT_PROCESS:
            if (buffer_pos > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READY;
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

state_t handle_processing(event_t event) {
    switch (event) {
        case EVENT_SUCCESS:
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

state_t handle_complete(event_t event) {
    switch (event) {
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            buffer_pos = 0;
            return STATE_IDLE;
        default:
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

void process_event(event_t event) {
    if (current_state >= STATE_COUNT || event >= EVENT_COUNT) {
        return;
    }
    state_t new_state = state_handlers[current_state](event);
    if (new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

void print_state_info(void) {
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[current_state]);
        if (current_state == STATE_READY || current_state == STATE_COMPLETE) {
            printf("Buffer size: %zu\n", buffer_pos);
        }
    }
}

int main(void) {
    int choice;
    
    printf("State Machine Demo\n");
    printf("0: EVENT_START, 1: EVENT_DATA, 2: EVENT_PROCESS\n");
    printf("3: EVENT_SUCCESS, 4: EVENT_FAILURE, 5: EVENT_RESET\n");
    printf("6: Exit\n");
    
    while (1) {
        print_state_info();
        printf("Enter event (0-6): ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice < 0 || choice > 6) {
            printf("Invalid choice\n");
            continue;
        }
        
        if (choice == 6) {
            break;
        }
        
        process_event((event_t)choice);
    }
    
    return 0;
}