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
    if (sm == NULL || sm->buffer_len >= sizeof(sm->buffer)) return -1;
    if (sm->buffer_len == 0) return -1;
    printf("Processing %zu bytes of data\n", sm->buffer_len);
    sm->data_count++;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Writing processed data\n");
    sm->processed_count++;
    return 0;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Processing complete. Data: %u, Processed: %u\n", 
           sm->data_count, sm->processed_count);
    return 0;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_count++;
    printf("Error occurred. Total errors: %u\n", sm->error_count);
    return 0;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Resetting from error state\n");
    sm->buffer_len = 0;
    return 0;
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
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                int result = transitions[i].action(sm);
                if (result != 0) return result;
            }
            sm->current_state = transitions[i].next;
            return 0;
        }
    }
    return -1;
}

void simulate_data_input(struct StateMachine* sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return;
    
    const char* sample_data = "Sample data packet";
    size_t data_len = strlen(sample_data);
    if (data_len < sizeof(sm->buffer)) {
        memcpy(sm->buffer, sample_data, data_len);
        sm->buffer_len = data_len;
        printf("Data read: %s\n", sm->buffer);
    }
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_count = 0;
    sm.processed_count = 0;
    sm.error_count = 0;
    sm.buffer_len = 0;
    
    int running = 1;
    char input[32];
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");
    
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
        
        if (fgets(input, sizeof(input