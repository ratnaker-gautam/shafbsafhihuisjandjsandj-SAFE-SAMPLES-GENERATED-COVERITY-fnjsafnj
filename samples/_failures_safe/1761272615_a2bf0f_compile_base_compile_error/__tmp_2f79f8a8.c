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

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Operation complete. Total data processed: %u\n", sm->data_count);
    sm->buffer_len = 0;
    return 0;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_count++;
    printf("Error occurred. Total errors: %u\n", sm->error_count);
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
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                int result = transitions[i].action(sm);
                if (result == 0) {
                    sm->current_state = transitions[i].next;
                }
                return result;
            }
        }
    }
    return -1;
}

int read_input_data(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    
    printf("Enter data to process (max 255 characters): ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return -1;
    }
    
    sm->buffer_len = strlen(sm->buffer);
    if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
        sm->buffer[--sm->buffer_len] = '\0';
    }
    
    if (sm->buffer_len == 0) {
        return -1;
    }
    
    return 0;
}

void print_menu(void) {
    printf("\nState Machine Menu:\n");
    printf("1. Start processing\n");
    printf("2. Enter data\n");
    printf("3. Process data\n");
    printf("4