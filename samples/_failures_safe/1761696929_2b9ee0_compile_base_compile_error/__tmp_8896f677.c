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
    if (sm == NULL) return -1;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    printf("Starting data processing...\n");
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return -1;
    printf("Processing %zu bytes of data\n", sm->buffer_len);
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->processed_count++;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return -1;
    printf("Writing processed data: ");
    for (size_t i = 0; i < sm->buffer_len && i < sizeof(sm->buffer); i++) {
        putchar(sm->buffer[i]);
    }
    printf("\n");
    sm->data_count++;
    return 0;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_count++;
    printf("Error occurred! Total errors: %u\n", sm->error_count);
    return 0;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    printf("State machine reset to idle\n");
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING, NULL},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (transitions[i].action(sm) != 0) {
                    return -1;
                }
            }
            sm->current_state = transitions[i].next;
            return 0;
        }
    }
    return -1;
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) return names[state];
    return "UNKNOWN";
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_count = 0;
    sm.processed_count = 0;
    sm.error_count = 0;
    sm.buffer_len = 0;
    
    int running = 1;
    char input[256];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");
    
    while (running) {
        printf("\nCurrent state: %s | Data: %u | Processed: %u | Errors: %u\n",
               state_to_string(sm.current_state), sm.data_count, sm.processed_count, sm.error_count);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len