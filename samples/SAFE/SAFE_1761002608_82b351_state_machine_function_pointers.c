//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
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

typedef void (*StateHandler)(void);

static State current_state = STATE_IDLE;
static int data_value = 0;
static int process_count = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void handle_ready(void) {
    printf("State: READY - Data loaded: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Count: %d\n", process_count);
    process_count++;
}

void handle_done(void) {
    printf("State: DONE - Processing complete\n");
}

void handle_error(void) {
    printf("State: ERROR - Operation failed\n");
}

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_done,
        handle_error
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        return handlers[state];
    }
    return handle_error;
}

State transition(Event event, int data) {
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                if (data >= 0 && data <= 100) {
                    data_value = data;
                    return STATE_READY;
                }
                return STATE_ERROR;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (data >= 0 && data <= 100) {
                    data_value = data;
                    return STATE_READY;
                }
                return STATE_ERROR;
            } else if (event == EVENT_PROCESS) {
                process_count = 0;
                return STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                return STATE_DONE;
            } else if (event == EVENT_FAILURE) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                data_value = 0;
                process_count = 0;
                return STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                data_value = 0;
                process_count = 0;
                return STATE_IDLE;
            }
            break;
    }
    
    return current_state;
}

int get_valid_input(int min, int max) {
    int value;
    char input[32];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &value) != 1) {
        return -1;
    }
    
    if (value < min || value > max) {
        return -1;
    }
    
    return value;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        StateHandler handler = get_handler(current_state);
        handler();
        
        printf("Enter event (-1 to exit): ");
        int event_input = get_valid_input(-1, 5);
        
        if (event_input == -1) {
            break;
        }
        
        Event event = (Event)event_input;
        int data = 0;
        
        if (event == EVENT_START || event == EVENT_DATA) {
            printf("Enter data (0-100): ");
            data = get_valid_input(0, 100);
            if (data == -1) {
                printf("Invalid data\n");
                continue;
            }
        }
        
        State new_state = transition(event, data);
        
        if (new_state == current_state) {
            printf("Invalid transition from current state\n");
        } else {
            current_state = new_state;
        }
        
        printf("\n");
    }
    
    return 0;
}