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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t error_count;
    uint32_t max_errors;
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
    
    table->next_state[STATE_PROCESSING][EVENT_PROCESS_DONE] = STATE_WRITING;
    table->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_WRITING][EVENT_WRITE_DONE] = STATE_IDLE;
    table->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    table->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    table->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

void initialize_state_machine(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->max_errors = 3;
}

int validate_event(enum Event event) {
    return event >= 0 && event < EVENT_COUNT;
}

int validate_state(enum State state) {
    return state >= 0 && state < STATE_COUNT;
}

int handle_event(struct StateMachine *sm, struct TransitionTable *table, enum Event event) {
    if (!validate_event(event) || !validate_state(sm->current_state)) {
        return 0;
    }
    
    enum State next_state = table->next_state[sm->current_state][event];
    if (!validate_state(next_state)) {
        return 0;
    }
    
    switch (event) {
        case EVENT_START:
            if (sm->current_state == STATE_IDLE) {
                sm->data_value = 1;
            }
            break;
            
        case EVENT_DATA_READY:
            if (sm->current_state == STATE_READING) {
                sm->data_value += 10;
            }
            break;
            
        case EVENT_PROCESS_DONE:
            if (sm->current_state == STATE_PROCESSING) {
                sm->processed_value = sm->data_value * 2;
            }
            break;
            
        case EVENT_WRITE_DONE:
            if (sm->current_state == STATE_WRITING) {
                printf("Output: %u\n", sm->processed_value);
            }
            break;
            
        case EVENT_ERROR:
            sm->error_count++;
            if (sm->error_count >= sm->max_errors) {
                printf("Too many errors, stopping.\n");
                return 0;
            }
            break;
            
        case EVENT_RESET:
            sm->error_count = 0;
            sm->data_value = 0;
            sm->processed_value = 0;
            break;
            
        default:
            return 0;
    }
    
    sm->current_state = next_state;
    return 1;
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state >= 0 && state < STATE_COUNT) {
        return names[state];
    }
    return "UNKNOWN";
}

const char* event_to_string(enum Event event) {
    static const char* names[] = {
        "START", "DATA_READY", "PROCESS_DONE", "WRITE_DONE", "ERROR", "RESET"
    };
    if (event >= 0 && event < EVENT_COUNT) {
        return names[event];
    }
    return "UNKNOWN";
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable table;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&table);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA_READY, 2=PROCESS_DONE, 3=WRITE_DONE, 4=ERROR, 5=RESET