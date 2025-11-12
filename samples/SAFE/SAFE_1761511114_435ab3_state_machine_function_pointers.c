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
    sm->max_operations = 5;
}

int handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    enum State new_state = transition_table[sm->current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) return 0;
    
    sm->current_state = new_state;
    
    if (sm->current_state >= 0 && sm->current_state < STATE_COUNT) {
        state_table[sm->current_state]();
    }
    
    if (event == EVENT_PROCESS && sm->current_state == STATE_PROCESSING) {
        if (sm->data_processed < sm->max_operations) {
            sm->data_processed++;
            printf("Processed data unit %d/%d\n", sm->data_processed, sm->max_operations);
        }
    }
    
    return 1;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA_READY\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    struct StateMachine sm;
    initialize_machine(&sm);
    
    printf("State Machine Demo Started\n");
    state_table[sm.current_state]();
    
    int running = 1;
    while (running) {
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event choice.\n");
            continue;
        }
        
        if (!handle_event(&sm, (enum Event)choice)) {
            printf("Event handling failed.\n");
        }
        
        if (sm.data_processed >= sm.max_operations && sm.current_state == STATE_PROCESSING) {
            printf("All data processed. Triggering success.\n");
            handle_event(&sm, EVENT_SUCCESS);
        }
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}