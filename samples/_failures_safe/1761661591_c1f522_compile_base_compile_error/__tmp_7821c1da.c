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
    uint32_t error_count;
    uint32_t total_operations;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_value = 0;
    return 1;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_value = (sm->data_value * 17 + 23) % 1000;
    return 1;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->processed_value = sm->data_value * 2;
    if (sm->processed_value < sm->data_value) return 0;
    sm->total_operations++;
    return 1;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Processed value: %u\n", sm->processed_value);
    return 1;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->error_count++;
    return 1;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_error_to_idle}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (!transitions[i].action(sm)) {
                    return 0;
                }
            }
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[sm->current_state]);
    } else {
        printf("Current state: UNKNOWN\n");
    }
    printf("Data value: %u\n", sm->data_value);
    printf("Processed value: %u\n", sm->processed_value);
    printf("Error count: %u\n", sm->error_count);
    printf("Total operations: %u\n", sm->total_operations);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.processed_value = 0;
    sm.error_count = 0;
    sm.total_operations = 0;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=QUIT\n");
    
    int running = 1;
    while (running) {
        print_state_info(&sm);
        
        printf("Enter command: ");
        int command;
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (command < 0 || command > 6) {
            printf("Invalid command\n");
            continue;
        }
        
        if (command ==