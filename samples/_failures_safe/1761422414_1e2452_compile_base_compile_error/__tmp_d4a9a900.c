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
    printf("Read %zu bytes of data\n", sm->buffer_len);
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->processed_count++;
    printf("Processed data chunk %u\n", sm->processed_count);
    return 0;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Write completed. Total processed: %u\n", sm->processed_count);
    return 0;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_count++;
    printf("Error occurred. Error count: %u\n", sm->error_count);
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

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < transition_count; i++) {
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

void simulate_data_read(struct StateMachine* sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return;
    
    const char* sample_data = "Sample data chunk for processing";
    size_t data_len = strlen(sample_data);
    
    if (data_len < sizeof(sm->buffer)) {
        memcpy(sm->buffer, sample_data, data_len);
        sm->buffer_len = data_len;
        process_event(sm, EVENT_DATA_READY);
    }
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_count = 0;
    sm.processed_count = 0;
    sm.error_count = 0;
    sm.buffer_len = 0;
    
    printf("State Machine Demo - Processing Pipeline\n");
    printf("========================================\n");
    
    if (process_event(&sm, EVENT_START) == 0) {
        simulate_data_read(&sm);
        
        if (process_event(&sm, EVENT_PROCESS_COMPLETE) == 0) {
            process_event(&sm, EVENT_WRITE_COMPLETE);
        }
    }
    
    printf("\nSimulating error scenario:\n");
    if (process_event(&sm, EVENT_START) == 0) {
        process_event(&sm, EVENT_ERROR);
        process_event(&sm, EVENT_RESET);
    }
    
    printf("\nFinal state: ");
    switch (sm.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: