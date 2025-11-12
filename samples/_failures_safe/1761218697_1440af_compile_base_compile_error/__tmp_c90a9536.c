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

int action_idle_start(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State machine initialized\n");
    return 1;
}

int action_reading_data(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos >= sizeof(sm->buffer) - 1) return 0;
    
    char input[16];
    printf("Enter data (max 15 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
    
    if (sm->buffer_pos + len < sizeof(sm->buffer)) {
        strcpy(sm->buffer + sm->buffer_pos, input);
        sm->buffer_pos += len;
        sm->data_count++;
        printf("Data stored: %s (total: %u)\n", input, sm->data_count);
        return 1;
    }
    return 0;
}

int action_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    printf("Processing data: ");
    for (uint32_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
        putchar(sm->buffer[i]);
    }
    printf("\n");
    return 1;
}

int action_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Writing processed data to output\n");
    printf("OUTPUT: %s\n", sm->buffer);
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State machine reset\n");
    return 1;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_start},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_data},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (transitions[i].action(sm)) {
                    sm->current_state = transitions[i].next;
                    return 1;
                }
            }
        }
    }
    return 0;
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) return names[state];
    return "UNKNOWN";
}

const char* event_to_string(enum Event event) {
    static const char* names[] = {
        "START", "DATA_READY", "PRO