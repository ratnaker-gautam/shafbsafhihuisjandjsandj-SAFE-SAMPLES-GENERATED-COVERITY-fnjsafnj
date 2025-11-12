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

State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void handle_idle(void) {
    printf("State: IDLE\n");
    printf("Enter command (start/reset/quit): ");
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100) or command (reset/quit): ");
}

void handle_processing(void) {
    printf("State: PROCESSING - Value: %d\n", data_value);
    printf("Processing... ");
    if (data_value > 0 && data_value <= 100) {
        int result = data_value * 2;
        if (result > 0 && result <= 200) {
            printf("Result: %d\n", result);
            current_state = STATE_COMPLETE;
        } else {
            current_state = STATE_ERROR;
        }
    } else {
        current_state = STATE_ERROR;
    }
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processed %d times\n", process_count);
    printf("Enter command (reset/quit): ");
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Enter command (reset/quit): ");
}

Event get_event_from_input(const char* input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return EVENT_COUNT;
    return EVENT_COUNT;
}

void transition(Event event) {
    State new_state = current_state;
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) new_state = STATE_READY;
            break;
        case STATE_READY:
            if (event == EVENT_DATA) new_state = STATE_READY;
            else if (event == EVENT_PROCESS) new_state = STATE_PROCESSING;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) new_state = STATE_COMPLETE;
            else if (event == EVENT_FAILURE) new_state = STATE_ERROR;
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) new_state = STATE_IDLE;
            break;
        default:
            break;
    }
    
    if (new_state != current_state) {
        current_state = new_state;
        if (current_state == STATE_IDLE) {
            data_value = 0;
            process_count = 0;
        }
    }
}

int main(void) {
    char input[256];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Type 'quit' to exit\n");
    
    while (running) {
        if (current_state < 0 || current_state >= STATE_COUNT) {
            current_state = STATE_IDLE;
        }
        
        state_handlers[current_state]();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        Event event = get_event_from_input(input);
        
        if (current_state == STATE_READY && event == EVENT_COUNT) {
            char* endptr;
            long value = strtol(input, &endptr, 10);
            if (endptr != input && *endptr == '\0' && value >= 0 && value <= 100) {
                data_value = (int)value;
                event = EVENT_DATA;
            }
        }
        
        if (event < EVENT_COUNT) {
            transition(event);
        } else {
            printf("Invalid input for current state\n");
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}