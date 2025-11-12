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
};

static const struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_START, STATE_READING}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State machine is idle. Send START event to begin.\n");
            break;
            
        case STATE_READING:
            if (sm->data_count < 100) {
                snprintf(sm->buffer, sizeof(sm->buffer), "Data packet %u", sm->data_count);
                sm->buffer_len = strlen(sm->buffer);
                sm->data_count++;
                printf("Read: %s\n", sm->buffer);
                state_machine_handle_event(sm, EVENT_DATA_READY);
            } else {
                printf("All data processed successfully!\n");
                sm->current_state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->buffer_len > 0 && sm->buffer_len < sizeof(sm->buffer)) {
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
                sm->processed_count++;
                printf("Processed: %s\n", sm->buffer);
                state_machine_handle_event(sm, EVENT_PROCESS_COMPLETE);
            } else {
                state_machine_handle_event(sm, EVENT_ERROR);
            }
            break;
            
        case STATE_WRITING:
            printf("Writing: %s\n", sm->buffer);
            sm->buffer_len = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            state_machine_handle_event(sm, EVENT_WRITE_COMPLETE);
            break;
            
        case STATE_ERROR:
            sm->error_count++;
            printf("Error occurred. Error count: %u\n", sm->error_count);
            break;
            
        default:
            break;
    }
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) {
        return names[state];
    }
    return "UNKNOWN";
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    int running = 1;
    char input[32];
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, write, error, reset, quit\n\n");
    
    while (running) {
        printf("Current state: %s | Data: %u | Processed: %u | Errors: %u\n",
               state_to_string(sm.current_state),
               sm.data_count, sm.processed_count, sm.error_count);
        
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }