//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

static void process_buffer(StateMachine *sm) {
    if (sm->buffer_pos == 0) return;
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isdigit(sm->buffer[i])) {
            int digit = sm->buffer[i] - '0';
            if (sum > INT_MAX - digit) {
                sm->result = -1;
                return;
            }
            sum += digit;
        }
    }
    sm->result = sum;
}

static void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static void run_state_machine(StateMachine *sm, const char *input) {
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (sm->current_state == STATE_READING) {
            if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_pos++] = c;
            } else {
                sm->current_state = STATE_ERROR;
                break;
            }
        }
        
        State next_state = get_next_state(sm->current_state, c);
        
        if (next_state == STATE_PROCESSING && sm->current_state == STATE_READING) {
            process_buffer(sm);
        } else if (next_state == STATE_IDLE && (sm->current_state == STATE_COMPLETE || sm->current_state == STATE_ERROR)) {
            reset_machine(sm);
        } else {
            sm->current_state = next_state;
        }
        
        if (sm->current_state == STATE_ERROR) {
            break;
        }
    }
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter command sequence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    run_state_machine(&sm, input);
    
    switch (sm.current_state) {
        case STATE_IDLE:
            printf("Machine reset\n");
            break;
        case STATE_READING:
            printf("Still reading input\n");
            break;
        case STATE_PROCESSING:
            printf("Processing data\n");
            break;
        case STATE_COMPLETE:
            printf("Processing complete. Result: %d\n", sm.result);
            break;
        case STATE_ERROR:
            printf("Error occurred during processing\n");
            break;
    }
    
    return 0;
}