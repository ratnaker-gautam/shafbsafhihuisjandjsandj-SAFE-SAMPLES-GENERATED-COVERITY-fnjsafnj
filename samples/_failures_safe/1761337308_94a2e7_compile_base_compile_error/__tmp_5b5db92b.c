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
};

static const struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && 
            transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_READING) return 0;
    
    if (sm->data_count >= sm->max_data) {
        return 0;
    }
    
    printf("Enter data (max 255 chars): ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    sm->buffer_pos = 0;
    sm->data_count++;
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_PROCESSING) return 0;
    
    size_t len = strlen(sm->buffer);
    for (size_t i = 0; i < len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_WRITING) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

void print_state(enum State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    int running = 1;
    char input[32];
    
    printf("State Machine Demo - Commands: start, read, process, write, reset, quit\n");
    
    while (running) {
        print_state(sm.current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "start") == 0) {
            if (state_machine_handle_event(&sm, EVENT_START)) {
                printf("Transitioned to READING state\n");
            }
        } else if (strcmp(input, "read") == 0) {
            if (state_machine_handle_event(&sm, EVENT_DATA_READY)) {
                if (read_data(&sm)) {
                    printf("Data read successfully\n");
                } else {
                    state_machine_handle_event(&sm, EVENT_ERROR