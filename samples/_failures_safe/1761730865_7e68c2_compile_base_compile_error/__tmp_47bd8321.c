//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint8_t error_code;
    uint32_t cycle_count;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_transition_table(struct TransitionTable *table) {
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            table->next_state[i][j] = STATE_ERROR;
        }
    }
    
    table->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    table->next_state[STATE_IDLE][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    table->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_READING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    table->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    table->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void initialize_state_machine(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_code = 0;
    sm->cycle_count = 0;
}

int validate_event(enum Event event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(enum State state) {
    return state >= 0 && state < STATE_COUNT;
}

int handle_event(struct StateMachine *sm, struct TransitionTable *table, enum Event event) {
    if (!validate_state(sm->current_state) || !validate_event(event)) {
        return 0;
    }
    
    enum State next_state = table->next_state[sm->current_state][event];
    if (!validate_state(next_state)) {
        return 0;
    }
    
    sm->current_state = next_state;
    
    switch (event) {
        case EVENT_START:
            sm->data_value = (sm->cycle_count * 7) + 1;
            break;
        case EVENT_DATA_READY:
            sm->processed_value = sm->data_value * 2;
            break;
        case EVENT_PROCESS_COMPLETE:
            break;
        case EVENT_WRITE_COMPLETE:
            sm->cycle_count++;
            break;
        case EVENT_ERROR:
            sm->error_code = 1;
            break;
        case EVENT_RESET:
            sm->error_code = 0;
            sm->data_value = 0;
            sm->processed_value = 0;
            break;
        default:
            break;
    }
    
    return 1;
}

void print_state_info(struct StateMachine *sm) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (validate_state(sm->current_state)) {
        printf("State: %s, Data: %u, Processed: %u, Cycles: %u, Error: %u\n",
               state_names[sm->current_state],
               sm->data_value,
               sm->processed_value,
               sm->cycle_count,
               sm->error_code);
    }
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&table);
    
    printf("State Machine Simulation\n");
    printf("Commands: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=QUIT\n");
    
    int running = 1;
    while (running) {
        print_state_info(&sm);
        
        printf("Enter command: ");
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input < 0 || input > 6) {
            printf("Invalid command. Use 0-6.\n");
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;