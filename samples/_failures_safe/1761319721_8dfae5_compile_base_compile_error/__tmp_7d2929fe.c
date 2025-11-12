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

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE},
        {STATE_WRITING, EVENT_RESET, STATE_IDLE}
    };
    
    size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    
    return 0;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    if (sm->data_count >= sm->max_data) {
        return 0;
    }
    
    printf("Enter data item %u: ", sm->data_count + 1);
    
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    if (strlen(sm->buffer) == 0) {
        return 0;
    }
    
    sm->data_count++;
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    
    size_t len = strlen(sm->buffer);
    for (size_t i = 0; i < len; i++) {
        sm->buffer[i] = (char)toupper((unsigned char)sm->buffer[i]);
    }
    
    sm->processed_count++;
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS_COMPLETE: return "PROCESS_COMPLETE";
        case EVENT_WRITE_COMPLETE: return "WRITE_COMPLETE";
        case EVENT_ERROR: return "ERROR";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    char input[32];
    int running = 1;
    
    printf("State Machine Demo - Process up to %u data items\n", sm.max_data);
    printf("Commands: start, read, process, write, error, reset, quit\n");
    
    while (running && sm.processed_count < sm.max_data) {
        printf("\nCurrent state: