//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
};

typedef void (*StateHandler)(void);

void idle_handler(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void ready_handler(void) {
    printf("State: READY - Data loaded, ready to process\n");
}

void processing_handler(void) {
    printf("State: PROCESSING - Working on data\n");
}

void complete_handler(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void error_handler(void) {
    printf("State: ERROR - Operation failed\n");
}

StateHandler state_table[STATE_COUNT] = {
    idle_handler,
    ready_handler,
    processing_handler,
    complete_handler,
    error_handler
};

enum State transition_table[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,   STATE_IDLE, STATE_IDLE,     STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
    {STATE_READY,   STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR,  STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR,  STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,   STATE_ERROR,   STATE_ERROR,   STATE_ERROR,   STATE_ERROR,   STATE_IDLE}
};

struct StateMachine {
    enum State current_state;
    int data_processed;
    int max_operations;
};

void initialize_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_processed = 0;
    sm->max_operations = 10;
}

int validate_event(enum Event ev) {
    return ev >= 0 && ev < EVENT_COUNT;
}

int validate_state(enum State st) {
    return st >= 0 && st < STATE_COUNT;
}

void process_event(struct StateMachine *sm, enum Event ev) {
    if (sm == NULL || !validate_state(sm->current_state) || !validate_event(ev)) {
        return;
    }
    
    enum State next_state = transition_table[sm->current_state][ev];
    if (!validate_state(next_state)) {
        return;
    }
    
    sm->current_state = next_state;
    
    if (validate_state(sm->current_state)) {
        state_table[sm->current_state]();
    }
    
    if (sm->current_state == STATE_PROCESSING) {
        if (sm->data_processed < sm->max_operations) {
            sm->data_processed++;
            printf("Processed operation %d/%d\n", sm->data_processed, sm->max_operations);
        }
    }
}

enum Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("0: START\n");
    printf("1: DATA_READY\n");
    printf("2: PROCESS\n");
    printf("3: SUCCESS\n");
    printf("4: FAILURE\n");
    printf("5: RESET\n");
    printf("6: EXIT\n");
    printf("Choose event (0-6): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (choice >= 0 && choice <= 5) {
        return (enum Event)choice;
    }
    
    return EVENT_COUNT;
}

int main(void) {
    struct StateMachine sm;
    initialize_machine(&sm);
    
    printf("State Machine Demo Started\n");
    printf("Initial state: ");
    state_table[sm.current_state]();
    
    while (1) {
        enum Event ev = get_user_event();
        
        if (ev == EVENT_COUNT) {
            printf("Exiting state machine demo\n");
            break;
        }
        
        process_event(&sm, ev);
        
        if (sm.current_state == STATE_COMPLETE) {
            printf("All operations completed successfully!\n");
            process_event(&sm, EVENT_RESET);
        } else if (sm.current_state == STATE_ERROR) {
            printf("Operation failed, resetting...\n");
            process_event(&sm, EVENT_RESET);
        }
    }
    
    return 0;
}