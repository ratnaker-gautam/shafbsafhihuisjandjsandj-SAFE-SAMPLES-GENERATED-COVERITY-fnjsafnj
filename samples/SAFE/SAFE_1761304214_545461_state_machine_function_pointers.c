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
    printf("Options: 0-Start, 5-Exit\n");
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Current data: %d\n", data_value);
    printf("Options: 1-Enter data, 2-Process, 6-Reset\n");
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", data_value);
    
    if (data_value < 0) {
        current_state = STATE_ERROR;
        return;
    }
    
    int result = data_value * 2;
    if (result < data_value) {
        current_state = STATE_ERROR;
        return;
    }
    
    data_value = result;
    process_count++;
    
    printf("Processed result: %d (count: %d)\n", data_value, process_count);
    printf("Options: 3-Success, 4-Failure, 6-Reset\n");
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Final result: %d\n", data_value);
    printf("Total processes: %d\n", process_count);
    printf("Options: 6-Reset, 5-Exit\n");
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Invalid operation detected\n");
    printf("Options: 6-Reset, 5-Exit\n");
}

Event get_user_input(void) {
    int input;
    printf("Enter event code: ");
    
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (input < 0 || input >= EVENT_COUNT) {
        return EVENT_COUNT;
    }
    
    return (Event)input;
}

void transition(Event event) {
    State new_state = current_state;
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                new_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                printf("Enter data value (0-10000): ");
                if (scanf("%d", &data_value) == 1) {
                    if (data_value >= 0 && data_value <= 10000) {
                        new_state = STATE_READY;
                    } else {
                        new_state = STATE_ERROR;
                    }
                } else {
                    while (getchar() != '\n');
                    new_state = STATE_ERROR;
                }
            } else if (event == EVENT_PROCESS) {
                new_state = STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                new_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                new_state = STATE_ERROR;
            }
            break;
            
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                data_value = 0;
                process_count = 0;
                new_state = STATE_IDLE;
            }
            break;
            
        default:
            break;
    }
    
    if (event == EVENT_RESET && current_state != STATE_COMPLETE && current_state != STATE_ERROR) {
        data_value = 0;
        process_count = 0;
        new_state = STATE_IDLE;
    }
    
    current_state = new_state;
}

int main(void) {
    printf("State Machine Demo\n");
    printf("Event codes: 0-Start, 1-Data, 2-Process, 3-Success, 4-Failure, 5-Exit, 6-Reset\n");
    
    int running = 1;
    
    while (running) {
        state_handlers[current_state]();
        
        Event event = get_user_input();
        if (event == EVENT_COUNT) {
            printf("Invalid event code\n");
            continue;
        }
        
        if (event == 5) {
            running = 0;
            continue;
        }
        
        transition(event);
        printf("\n");
    }
    
    printf("Program terminated\n");
    return 0;
}