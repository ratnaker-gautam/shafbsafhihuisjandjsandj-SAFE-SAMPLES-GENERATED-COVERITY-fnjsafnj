//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static state_t current_state = STATE_IDLE;
static char buffer[256];
static size_t buffer_pos = 0;

static state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("Starting data processing...\n");
            buffer_pos = 0;
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

static state_t handle_reading(event_t event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (buffer_pos < sizeof(buffer) - 1) {
                buffer[buffer_pos++] = 'A' + (rand() % 26);
                buffer[buffer_pos] = '\0';
                printf("Read data: %s\n", buffer);
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_processing(event_t event) {
    switch (event) {
        case EVENT_PROCESS_DONE:
            for (size_t i = 0; i < buffer_pos && i < sizeof(buffer); i++) {
                if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                    buffer[i] = buffer[i] - 'a' + 'A';
                }
            }
            printf("Processed data: %s\n", buffer);
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

static state_t handle_writing(event_t event) {
    switch (event) {
        case EVENT_WRITE_DONE:
            printf("Data written successfully.\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

static state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting state machine...\n");
            buffer_pos = 0;
            memset(buffer, 0, sizeof(buffer));
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static state_handler_t handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

static event_t get_next_event(void) {
    static const event_t events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE,
        EVENT_ERROR,
        EVENT_RESET
    };
    static size_t event_index = 0;
    
    if (event_index >= sizeof(events) / sizeof(events[0])) {
        event_index = 0;
    }
    
    return events[event_index++];
}

int main(void) {
    srand(42);
    
    printf("State Machine Demo Started\n");
    
    for (int i = 0; i < 20; i++) {
        event_t event = get_next_event();
        
        if (current_state >= STATE_COUNT || event >= EVENT_COUNT) {
            printf("Invalid state or event\n");
            break;
        }
        
        state_t new_state = handlers[current_state](event);
        
        if (new_state >= STATE_COUNT) {
            printf("Invalid new state\n");
            break;
        }
        
        current_state = new_state;
        
        if (current_state == STATE_IDLE && i > 10) {
            break;
        }
    }
    
    printf("State Machine Demo Completed\n");
    return 0;
}