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
    
    printf("Enter data (max %u chars): ", (uint32_t)(sizeof(sm->buffer) - sm->buffer_pos - 1));
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (sm->buffer_pos + input_len >= sizeof(sm->buffer) - 1) {
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, input, input_len);
    sm->buffer_pos += input_len;
    sm->buffer[sm->buffer_pos] = '\0';
    sm->data_count++;
    
    printf("Data stored: %s\n", sm->buffer);
    return 1;
}

int action_processing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    printf("Data processed: %s\n", sm->buffer);
    return 1;
}

int action_writing(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Final output: %s\n", sm->buffer);
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
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return 0;
    
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

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("1 - START\n");
    printf("2 - DATA_READY\n");
    printf("3 - PROCESS_COMPLETE\n");
    printf("4 - WRITE_COMPLETE\n");
    printf("5 - ERROR\n");
    printf