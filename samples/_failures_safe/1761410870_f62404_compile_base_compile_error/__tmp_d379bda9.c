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

struct Transition {
    enum State from_state;
    enum Event event;
    enum State to_state;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_value = 0;
    return 1;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->data_value > 1000) return 0;
    sm->processed_value = sm->data_value * 2;
    return 1;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Writing value: %u\n", sm->processed_value);
    return 1;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 1;
}

int action_any_to_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->error_count++;
    printf("Error occurred. Total errors: %u\n", sm->error_count);
    return 1;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->error_count >= sm->max_errors) return 0;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_DONE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_DONE, STATE_IDLE, action_writing_to_idle},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_any_to_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_any_to_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_any_to_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_any_to_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_error_to_idle}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (!transitions[i].action(sm)) {
                    return 0;
                }
            }
            sm->current_state = transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void initialize_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->max_errors = 3;
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);
    
    char input[32];
    int running = 1;
    
    while (running) {
        printf("Current state: %d\n", sm.current_state);
        printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_DONE, 3=WRITE_DONE, 4=ERROR, 5=RESET, 6=QUIT): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char* endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice < 0 || choice > 6) {
            printf("Invalid event\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        enum Event event = (enum Event)choice;
        
        if (event == EVENT_DATA_READY && sm.current_state == STATE_READING) {
            printf("Enter data value (0-1000): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {