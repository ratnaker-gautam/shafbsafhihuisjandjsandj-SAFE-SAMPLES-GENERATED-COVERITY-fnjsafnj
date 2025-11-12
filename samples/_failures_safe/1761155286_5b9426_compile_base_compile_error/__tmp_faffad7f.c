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
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
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
    sm->buffer_pos = 0;
    printf("Starting data processing\n");
    return 1;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL || sm->buffer_pos == 0) return 0;
    printf("Processing %u bytes of data\n", sm->buffer_pos);
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL || sm->buffer_pos == 0) return 0;
    printf("Writing processed data: ");
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        putchar(sm->buffer[i]);
    }
    putchar('\n');
    sm->data_count++;
    return 1;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Operation complete. Total operations: %u\n", sm->data_count);
    if (sm->data_count >= sm->max_data) {
        printf("Maximum operations reached\n");
        return 0;
    }
    return 1;
}

int action_to_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

int action_error_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Resetting state machine\n");
    sm->data_count = 0;
    sm->buffer_pos = 0;
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_to_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_error_to_idle}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL && transitions[i].action(sm)) {
                sm->current_state = transitions[i].next;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

int read_input_data(struct StateMachine* sm) {
    if (sm == NULL || sm->buffer_pos >= sizeof(sm->buffer)) return 0;
    
    printf("Enter data to process (max %zu chars): ", sizeof(sm->buffer) - sm->buffer_pos - 1);
    
    char input[512];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) return 0;
    
    if (sm->buffer_pos + input_len >= sizeof(sm->buffer)) {
        input_len = sizeof(sm->buffer) - sm->buffer_pos - 1;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, input, input_len);
    sm->buffer_pos += input_len;
    sm->buffer[sm->buffer_pos] = '\0';
    
    return 1;