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

typedef struct {
    State current_state;
    StateHandler handlers[STATE_COUNT];
} StateMachine;

static int data_value = 0;
static int process_count = 0;

void idle_handler(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void ready_handler(void) {
    printf("State: READY - Ready to receive data\n");
}

void processing_handler(void) {
    if (data_value <= 0) {
        printf("State: PROCESSING - Invalid data, transitioning to ERROR\n");
        return;
    }
    
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    
    if (data_value > 1000) {
        printf("State: PROCESSING - Data too large, transitioning to ERROR\n");
        return;
    }
    
    int result = data_value * 2;
    if (result < data_value) {
        printf("State: PROCESSING - Integer overflow detected\n");
        return;
    }
    
    printf("State: PROCESSING - Result: %d\n", result);
    process_count++;
}

void complete_handler(void) {
    printf("State: COMPLETE - Processing completed successfully\n");
    printf("Total processes: %d\n", process_count);
}

void error_handler(void) {
    printf("State: ERROR - An error occurred\n");
}

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->handlers[STATE_IDLE] = idle_handler;
    sm->handlers[STATE_READY] = ready_handler;
    sm->handlers[STATE_PROCESSING] = processing_handler;
    sm->handlers[STATE_COMPLETE] = complete_handler;
    sm->handlers[STATE_ERROR] = error_handler;
}

int state_machine_transition(StateMachine *sm, Event event, int data) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                next_state = STATE_READY;
            }
            break;
            
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (data >= 0) {
                    data_value = data;
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                next_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                next_state = STATE_ERROR;
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                process_count = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                data_value = 0;
            }
            break;
            
        default:
            break;
    }
    
    if (next_state != sm->current_state) {
        sm->current_state = next_state;
        return 1;
    }
    
    return 0;
}

void state_machine_run(StateMachine *sm) {
    if (sm == NULL || sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return;
    }
    
    if (sm->handlers[sm->current_state] != NULL) {
        sm->handlers[sm->current_state]();
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event number and optional data (for DATA event):\n");
    printf("Enter -1 to exit\n");
    
    int event_input;
    int data_input;
    
    while (1) {
        state_machine_run(&sm);
        
        printf("> ");
        
        if (scanf("%d", &event_input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (event_input == -1) {
            break;
        }
        
        if (event_input < 0 || event_input >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }
        
        if (event_input == EVENT_DATA) {
            if (scanf("%d", &data_input) != 1) {
                while (getchar() != '\n');
                printf("Invalid data input\n");
                continue;
            }
            state_machine_transition