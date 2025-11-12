//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    State next;
    char trigger;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
    int error_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
}

int is_valid_transition(State current, State next, char trigger) {
    if (current == STATE_IDLE && next == STATE_READING && trigger == 'S') return 1;
    if (current == STATE_READING && next == STATE_PROCESSING && trigger == 'P') return 1;
    if (current == STATE_PROCESSING && next == STATE_WRITING && trigger == 'W') return 1;
    if (current == STATE_WRITING && next == STATE_DONE && trigger == 'D') return 1;
    if (current == STATE_READING && next == STATE_ERROR && trigger == 'E') return 1;
    if (current == STATE_PROCESSING && next == STATE_ERROR && trigger == 'E') return 1;
    if (current == STATE_ERROR && next == STATE_IDLE && trigger == 'R') return 1;
    if (current == STATE_DONE && next == STATE_IDLE && trigger == 'R') return 1;
    return 0;
}

State process_transition(StateMachine *sm, char trigger) {
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (trigger == 'S') next_state = STATE_READING;
            break;
        case STATE_READING:
            if (trigger == 'P') next_state = STATE_PROCESSING;
            else if (trigger == 'E') next_state = STATE_ERROR;
            break;
        case STATE_PROCESSING:
            if (trigger == 'W') next_state = STATE_WRITING;
            else if (trigger == 'E') next_state = STATE_ERROR;
            break;
        case STATE_WRITING:
            if (trigger == 'D') next_state = STATE_DONE;
            break;
        case STATE_ERROR:
            if (trigger == 'R') next_state = STATE_IDLE;
            break;
        case STATE_DONE:
            if (trigger == 'R') next_state = STATE_IDLE;
            break;
    }
    
    if (is_valid_transition(sm->current_state, next_state, trigger)) {
        sm->current_state = next_state;
        return next_state;
    }
    
    return sm->current_state;
}

void simulate_processing(StateMachine *sm) {
    if (sm->buffer_pos == 0) return;
    
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        for (int j = 0; j < 1000; j++) {
            sm->buffer[i] = (sm->buffer[i] + 1) % 128;
        }
    }
    sm->processed_count++;
}

void print_state_info(StateMachine *sm) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Buffer: %.*s\n", (int)sm->buffer_pos, sm->buffer);
    printf("Processed: %d, Errors: %d\n\n", sm->processed_count, sm->error_count);
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    char input[MAX_INPUT_LEN + 2];
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, P=Process, W=Write, D=Done, E=Error, R=Reset, Q=Quit\n");
    printf("In READING state, enter text to buffer\n\n");
    
    while (1) {
        print_state_info(&sm);
        
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char command = toupper(input[0]);
        
        if (command == 'Q') {
            break;
        }
        
        if (sm.current_state == STATE_READING && command != 'P' && command != 'E') {
            size_t input_len = strlen(input);
            if (input_len > 0 && sm.buffer_pos < MAX_INPUT_LEN) {
                size_t copy_len = MAX_INPUT_LEN - sm.buffer_pos - 1;
                if (input_len < copy_len) {
                    copy_len = input_len;
                }
                memcpy(sm.buffer + sm.buffer_pos