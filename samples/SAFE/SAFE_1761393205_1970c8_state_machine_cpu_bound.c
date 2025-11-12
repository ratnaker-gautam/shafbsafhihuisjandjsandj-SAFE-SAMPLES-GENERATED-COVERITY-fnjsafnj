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
    if (sm->buffer_pos == 0) {
        sm->result = 0;
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isdigit((unsigned char)sm->buffer[i])) {
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

static void run_state_machine(StateMachine *sm) {
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter sequence (S to start, E to end processing, C to complete, X for error, R to reset): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (sm->current_state) {
            case STATE_IDLE:
                if (c == 'S') {
                    sm->current_state = STATE_READING;
                    sm->buffer_pos = 0;
                    printf("State: READING\n");
                }
                break;
                
            case STATE_READING:
                if (c == 'E') {
                    sm->current_state = STATE_PROCESSING;
                    printf("State: PROCESSING\n");
                    process_buffer(sm);
                } else if (c == 'X') {
                    sm->current_state = STATE_ERROR;
                    printf("State: ERROR\n");
                } else if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buffer_pos++] = c;
                }
                break;
                
            case STATE_PROCESSING:
                if (c == 'C') {
                    sm->current_state = STATE_COMPLETE;
                    printf("State: COMPLETE - Result: %d\n", sm->result);
                } else if (c == 'X') {
                    sm->current_state = STATE_ERROR;
                    printf("State: ERROR\n");
                }
                break;
                
            case STATE_COMPLETE:
                if (c == 'R') {
                    reset_machine(sm);
                    printf("State: IDLE\n");
                }
                break;
                
            case STATE_ERROR:
                if (c == 'R') {
                    reset_machine(sm);
                    printf("State: IDLE\n");
                }
                break;
        }
        
        State next = get_next_state(sm->current_state, c);
        if (next != sm->current_state) {
            sm->current_state = next;
        }
    }
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    printf("State Machine Processor\n");
    printf("Commands: S(start), E(end input), C(complete), X(error), R(reset)\n");
    printf("During READING, enter digits to sum\n");
    
    for (int i = 0; i < 5; i++) {
        run_state_machine(&sm);
        
        if (sm.current_state == STATE_COMPLETE) {
            printf("Processing cycle %d completed\n", i + 1);
        }
    }
    
    printf("Finished processing cycles\n");
    return 0;
}