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
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE,      STATE_IDLE,      STATE_IDLE,      STATE_IDLE,      STATE_IDLE},
    {STATE_READY,    STATE_READY,     STATE_PROCESSING,STATE_READY,     STATE_ERROR,     STATE_IDLE},
    {STATE_PROCESSING,STATE_PROCESSING,STATE_PROCESSING,STATE_COMPLETE, STATE_ERROR,     STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_COMPLETE,  STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR,     STATE_ERROR,     STATE_ERROR,     STATE_ERROR,     STATE_IDLE}
};

State current_state = STATE_IDLE;
char buffer[256];
size_t buffer_pos = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
    printf("Enter 'start' to begin: ");
}

void handle_ready(void) {
    printf("State: READY - Enter data to process: ");
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data...\n");
    if (buffer_pos > 0) {
        int sum = 0;
        for (size_t i = 0; i < buffer_pos; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                sum += buffer[i] - '0';
            }
        }
        printf("Sum of digits in data: %d\n", sum);
        if (sum > 0) {
            current_state = transition_table[current_state][EVENT_SUCCESS];
        } else {
            current_state = transition_table[current_state][EVENT_FAILURE];
        }
    } else {
        current_state = transition_table[current_state][EVENT_FAILURE];
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing successful\n");
    printf("Enter 'reset' to start over: ");
}

void handle_error(void) {
    printf("State: ERROR - Processing failed\n");
    printf("Enter 'reset' to start over: ");
}

Event get_event_from_input(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return EVENT_COUNT;
}

int main(void) {
    char input[64];
    
    printf("Finite State Machine Demo\n");
    printf("Available commands: start, data, process, reset\n");
    
    while (1) {
        state_handlers[current_state]();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        Event event = get_event_from_input(input);
        
        if (event == EVENT_COUNT) {
            if (current_state == STATE_READY) {
                if (strlen(input) < sizeof(buffer) - buffer_pos) {
                    strcpy(buffer + buffer_pos, input);
                    buffer_pos += strlen(input);
                    event = EVENT_DATA;
                } else {
                    printf("Input too long, discarding\n");
                    continue;
                }
            } else {
                printf("Invalid command\n");
                continue;
            }
        }
        
        if (event >= 0 && event < EVENT_COUNT && current_state >= 0 && current_state < STATE_COUNT) {
            State new_state = transition_table[current_state][event];
            if (new_state != current_state) {
                if (new_state == STATE_IDLE) {
                    buffer_pos = 0;
                    memset(buffer, 0, sizeof(buffer));
                }
                current_state = new_state;
            }
        }
    }
    
    return 0;
}