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

enum State state_machine_transition(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return STATE_ERROR;
    
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
    
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return transitions[i].next;
        }
    }
    
    return sm->current_state;
}

int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    if (sm->data_count >= sm->max_data) {
        sm->current_state = STATE_DONE;
        return 0;
    }
    
    printf("Enter data (max 255 chars): ");
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        return 0;
    }
    
    sm->buffer_pos = strlen(sm->buffer);
    if (sm->buffer_pos > 0 && sm->buffer[sm->buffer_pos-1] == '\n') {
        sm->buffer[sm->buffer_pos-1] = '\0';
        sm->buffer_pos--;
    }
    
    if (sm->buffer_pos == 0) {
        return 0;
    }
    
    sm->data_count++;
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    sm->processed_count++;
    return 1;
}

int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    
    printf("Processed data: %s\n", sm->buffer);
    return 1;
}

void print_state(enum State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    printf("Current state: %s\n", state_names[state]);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    char input[32];
    int running = 1;
    
    printf("State Machine Controller\n");
    printf("Commands: start, data, process, write, error, reset, quit\n");
    
    while (running) {
        print_state(sm.current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strcmp(input, "start") == 0) {
            state_machine_transition(&sm, EVENT_START);
        } else if (strcmp(input, "data") == 0) {
            if (read_data(&sm)) {
                state_machine_transition(&sm, EVENT_DATA_READY);
            }
        } else