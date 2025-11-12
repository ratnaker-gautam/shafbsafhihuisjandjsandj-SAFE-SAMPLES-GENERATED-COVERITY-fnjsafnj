//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
    uint32_t processed_count;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

struct StateMachine* create_state_machine(uint32_t max_data) {
    if (max_data == 0 || max_data > 1000) {
        return NULL;
    }
    
    struct StateMachine* sm = malloc(sizeof(struct StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    
    return sm;
}

void destroy_state_machine(struct StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
}

int handle_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) {
        return 0;
    }
    
    struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE}
    };
    
    size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    enum State new_state = sm->current_state;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            new_state = transitions[i].next;
            break;
        }
    }
    
    if (new_state == sm->current_state) {
        return 0;
    }
    
    sm->current_state = new_state;
    
    switch (sm->current_state) {
        case STATE_READING:
            if (sm->data_count >= sm->max_data) {
                sm->current_state = STATE_DONE;
            } else {
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0 && sm->buffer_pos < sizeof(sm->buffer)) {
                for (uint32_t i = 0; i < sm->buffer_pos; i++) {
                    sm->buffer[i] = toupper(sm->buffer[i]);
                }
                sm->processed_count++;
            }
            break;
        case STATE_WRITING:
            if (sm->buffer_pos > 0 && sm->buffer_pos < sizeof(sm->buffer)) {
                printf("Processed data: %s\n", sm->buffer);
                sm->data_count++;
            }
            break;
        case STATE_IDLE:
            sm->data_count = 0;
            sm->processed_count = 0;
            sm->buffer_pos = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            break;
        case STATE_DONE:
            printf("Processing complete. Total items: %u\n", sm->processed_count);
            break;
        case STATE_ERROR:
            printf("Error occurred in state machine\n");
            break;
    }
    
    return 1;
}

int read_data(struct StateMachine* sm, const char* data) {
    if (sm == NULL || data == NULL || sm->current_state != STATE_READING) {
        return 0;
    }
    
    size_t data_len = strlen(data);
    if (data_len == 0 || data_len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    if (sm->buffer_pos + data_len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, data, data_len);
    sm->buffer_pos += data_len;
    
    return 1;
}

int main(void) {
    struct StateMachine* sm = create_state_machine(5);
    if (sm == NULL) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    const char* test_data[] = {"hello", "world", "test", "data", "final"};
    size_t num_tests = sizeof(test_data) / sizeof(test_data[0]);
    
    handle_event(sm, EVENT_START);
    
    for (size_t i = 0