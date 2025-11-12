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
    uint32_t data_count;
    uint32_t processed_count;
    uint32_t error_count;
    char buffer[256];
    size_t buffer_len;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    printf("Transition: IDLE -> READING\n");
    return 1;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_len == 0 || sm->buffer_len >= sizeof(sm->buffer)) return 0;
    sm->data_count++;
    printf("Data received: %s\n", sm->buffer);
    return 1;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    for (size_t i = 0; i < sm->buffer_len && i < sizeof(sm->buffer) - 1; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->processed_count++;
    printf("Data processed: %s\n", sm->buffer);
    return 1;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Data written successfully. Processed: %u\n", sm->processed_count);
    return 1;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->error_count++;
    printf("Error occurred. Total errors: %u\n", sm->error_count);
    return 1;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Reset from error state. Errors: %u\n", sm->error_count);
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

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL && transitions[i].action(sm)) {
                sm->current_state = transitions[i].next;
                return 1;
            }
        }
    }
    return 0;
}

void initialize_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);
    
    char input[256];
    int running = 1;
    
    printf("State Machine Demo - Commands: start, data <text>, process, write, error, reset, quit\n");
    
    while (running) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin)