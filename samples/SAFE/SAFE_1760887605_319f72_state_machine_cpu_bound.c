//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    
    sm->result = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        sm->result += (int)sm->buffer[i];
        sm->result *= 31;
    }
}

static void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static int is_valid_input(char c) {
    return isalnum((unsigned char)c) || c == 'S' || c == 'E' || c == 'C' || c == 'X' || c == 'R';
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    printf("State Machine Processor\n");
    printf("Commands: S(start), E(end input), C(complete), X(error), R(reset)\n");
    printf("Alphanumeric characters are stored as data\n");
    printf("Enter commands: ");
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (!is_valid_input(c)) {
            continue;
        }
        
        State next_state = get_next_state(sm.current_state, c);
        
        switch (sm.current_state) {
            case STATE_IDLE:
                if (next_state == STATE_READING) {
                    reset_machine(&sm);
                    sm.current_state = STATE_READING;
                }
                break;
                
            case STATE_READING:
                if (next_state == STATE_PROCESSING) {
                    sm.current_state = STATE_PROCESSING;
                } else if (next_state == STATE_ERROR) {
                    sm.current_state = STATE_ERROR;
                    printf("Error: Invalid input sequence\n");
                } else if (isalnum((unsigned char)c) && sm.buffer_pos < MAX_INPUT_LEN - 1) {
                    sm.buffer[sm.buffer_pos++] = c;
                }
                break;
                
            case STATE_PROCESSING:
                if (next_state == STATE_COMPLETE) {
                    process_buffer(&sm);
                    sm.current_state = STATE_COMPLETE;
                    printf("Processing complete. Result: %d\n", sm.result);
                } else if (next_state == STATE_ERROR) {
                    sm.current_state = STATE_ERROR;
                    printf("Error: Processing failed\n");
                }
                break;
                
            case STATE_COMPLETE:
            case STATE_ERROR:
                if (next_state == STATE_IDLE) {
                    reset_machine(&sm);
                    printf("Machine reset\n");
                }
                break;
        }
    }
    
    printf("Final state: ");
    switch (sm.current_state) {
        case STATE_IDLE: printf("IDLE\n"); break;
        case STATE_READING: printf("READING\n"); break;
        case STATE_PROCESSING: printf("PROCESSING\n"); break;
        case STATE_COMPLETE: printf("COMPLETE\n"); break;
        case STATE_ERROR: printf("ERROR\n"); break;
    }
    
    return 0;
}