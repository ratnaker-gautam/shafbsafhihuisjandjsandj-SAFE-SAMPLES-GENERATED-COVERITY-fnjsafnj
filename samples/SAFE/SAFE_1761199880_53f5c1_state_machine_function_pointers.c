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
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    int data_value;
    int error_code;
};

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

struct StateMachine sm;

void handle_idle(void) {
    printf("State: IDLE\n");
    printf("Available events: 0=START\n");
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Data value: %d\n", sm.data_value);
    printf("Available events: 1=DATA, 2=PROCESS\n");
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", sm.data_value);
    printf("Available events: 3=SUCCESS, 4=FAILURE\n");
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Final result: %d\n", sm.data_value);
    printf("Available events: 5=RESET\n");
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Error code: %d\n", sm.error_code);
    printf("Available events: 5=RESET\n");
}

int process_event(Event event, int value) {
    switch (sm.current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm.current_state = STATE_READY;
                return 1;
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (value >= 0 && value <= 100) {
                    sm.data_value = value;
                    return 1;
                }
            } else if (event == EVENT_PROCESS) {
                if (sm.data_value >= 0) {
                    sm.current_state = STATE_PROCESSING;
                    return 1;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                sm.data_value *= 2;
                sm.current_state = STATE_COMPLETE;
                return 1;
            } else if (event == EVENT_FAILURE) {
                sm.error_code = value;
                sm.current_state = STATE_ERROR;
                return 1;
            }
            break;
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm.current_state = STATE_IDLE;
                sm.data_value = 0;
                sm.error_code = 0;
                return 1;
            }
            break;
        default:
            break;
    }
    return 0;
}

int main(void) {
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.error_code = 0;
    
    printf("State Machine Demo\n");
    printf("Enter events (0-5) and optional values\n");
    printf("Events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        state_handlers[sm.current_state]();
        
        int event_input;
        printf("\nEnter event: ");
        if (scanf("%d", &event_input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (event_input == -1) {
            break;
        }
        
        if (event_input < 0 || event_input >= NUM_EVENTS) {
            printf("Invalid event\n");
            continue;
        }
        
        Event event = (Event)event_input;
        int value = 0;
        
        if (event == EVENT_DATA || event == EVENT_FAILURE) {
            printf("Enter value: ");
            if (scanf("%d", &value) != 1) {
                while (getchar() != '\n');
                printf("Invalid value\n");
                continue;
            }
        }
        
        if (!process_event(event, value)) {
            printf("Event not allowed in current state\n");
        }
        
        printf("\n");
    }
    
    return 0;
}