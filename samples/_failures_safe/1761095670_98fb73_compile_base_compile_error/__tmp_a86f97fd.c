//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
    uint32_t error_count;
    uint32_t operation_count;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->operation_count = 0;
}

void initialize_transition_table(struct TransitionTable *tt) {
    if (tt == NULL) return;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            tt->next_state[i][j] = STATE_ERROR;
        }
    }
    
    tt->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    tt->next_state[STATE_IDLE][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    tt->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_READING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    tt->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

int handle_event(struct StateMachine *sm, const struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL) return 0;
    if (event < 0 || event >= EVENT_COUNT) return 0;
    
    enum State current = sm->current_state;
    enum State next = tt->next_state[current][event];
    
    if (next == STATE_ERROR && current != STATE_ERROR) {
        sm->error_count++;
        return 0;
    }
    
    sm->current_state = next;
    
    switch (event) {
        case EVENT_DATA_READY:
            sm->data_value = (sm->operation_count * 7 + 3) % 100;
            break;
        case EVENT_PROCESS_COMPLETE:
            sm->processed_value = sm->data_value * 2;
            break;
        case EVENT_WRITE_COMPLETE:
            sm->operation_count++;
            break;
        case EVENT_RESET:
            sm->data_value = 0;
            sm->processed_value = 0;
            break;
        default:
            break;
    }
    
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Value: %u\n", sm->data_value);
    printf("Processed Value: %u\n", sm->processed_value);
    printf("Error Count: %u\n", sm->error_count);
    printf("Operation Count: %u\n", sm->operation_count);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    struct TransitionTable tt;
    
    initialize_state_machine(&sm);
    initialize_transition_table(&tt);
    
    printf("State Machine Started\n");
    print_state_info(&sm);
    
    int running = 1;
    char input[32];
    
    while (running) {
        printf("Enter event (start, data, process, write, error, reset, quit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        enum Event event;
        
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "data") == 0) {
            event = EVENT_DATA_READY;
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS_COMPLETE;
        } else if (strcmp(input, "write") == 0) {
            event = EVENT_WRITE_COMPLETE