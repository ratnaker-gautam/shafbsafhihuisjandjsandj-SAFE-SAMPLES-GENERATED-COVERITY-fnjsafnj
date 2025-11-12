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

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_event(enum State current, enum Event event) {
    switch (current) {
        case STATE_IDLE:
            return event == EVENT_START || event == EVENT_RESET;
        case STATE_READING:
            return event == EVENT_DATA_READY || event == EVENT_ERROR || event == EVENT_RESET;
        case STATE_PROCESSING:
            return event == EVENT_PROCESS_COMPLETE || event == EVENT_ERROR || event == EVENT_RESET;
        case STATE_WRITING:
            return event == EVENT_WRITE_COMPLETE || event == EVENT_ERROR || event == EVENT_RESET;
        case STATE_ERROR:
            return event == EVENT_RESET;
        case STATE_DONE:
            return event == EVENT_RESET;
        default:
            return 0;
    }
}

enum State get_next_state(enum State current, enum Event event) {
    switch (current) {
        case STATE_IDLE:
            if (event == EVENT_START) return STATE_READING;
            break;
        case STATE_READING:
            if (event == EVENT_DATA_READY) return STATE_PROCESSING;
            if (event == EVENT_ERROR) return STATE_ERROR;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) return STATE_WRITING;
            if (event == EVENT_ERROR) return STATE_ERROR;
            break;
        case STATE_WRITING:
            if (event == EVENT_WRITE_COMPLETE) return STATE_READING;
            if (event == EVENT_ERROR) return STATE_ERROR;
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
        case STATE_DONE:
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
        default:
            break;
    }
    return current;
}

int handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (!is_valid_event(sm->current_state, event)) return 0;
    
    enum State next_state = get_next_state(sm->current_state, event);
    
    switch (event) {
        case EVENT_START:
            if (sm->current_state == STATE_IDLE) {
                sm->data_count = 0;
                sm->processed_count = 0;
                sm->buffer_pos = 0;
            }
            break;
        case EVENT_DATA_READY:
            if (sm->current_state == STATE_READING) {
                if (sm->data_count < sm->max_data) {
                    sm->data_count++;
                }
            }
            break;
        case EVENT_PROCESS_COMPLETE:
            if (sm->current_state == STATE_PROCESSING) {
                sm->processed_count++;
                if (sm->processed_count >= sm->max_data) {
                    next_state = STATE_DONE;
                }
            }
            break;
        case EVENT_WRITE_COMPLETE:
            break;
        case EVENT_ERROR:
            break;
        case EVENT_RESET:
            state_machine_init(sm);
            return 1;
        default:
            return 0;
    }
    
    sm->current_state = next_state;
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

int main() {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");
    
    char input[32];
    while (1) {
        printf("Current state: %s