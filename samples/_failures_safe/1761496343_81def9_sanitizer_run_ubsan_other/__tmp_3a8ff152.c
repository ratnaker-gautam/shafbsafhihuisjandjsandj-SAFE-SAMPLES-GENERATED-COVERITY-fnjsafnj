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
        if (isdigit(sm->buffer[i])) {
            sm->result += (sm->buffer[i] - '0');
        }
    }
}

static void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter commands (S: start, E: end input, C: complete, X: error, R: reset): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char cmd = input[i];
        
        if (cmd == 'S' && sm.current_state == STATE_IDLE) {
            sm.current_state = STATE_READING;
            sm.buffer_pos = 0;
            printf("State: READING - Enter digits to process\n");
        }
        else if (cmd == 'E' && sm.current_state == STATE_READING) {
            sm.current_state = STATE_PROCESSING;
            printf("State: PROCESSING - Processing buffer\n");
            process_buffer(&sm);
        }
        else if (cmd == 'C' && sm.current_state == STATE_PROCESSING) {
            sm.current_state = STATE_COMPLETE;
            printf("State: COMPLETE - Result: %d\n", sm.result);
        }
        else if (cmd == 'X') {
            sm.current_state = STATE_ERROR;
            printf("State: ERROR - Operation failed\n");
        }
        else if (cmd == 'R') {
            reset_machine(&sm);
            printf("State: IDLE - Machine reset\n");
        }
        else if (sm.current_state == STATE_READING && isdigit(cmd)) {
            if (sm.buffer_pos < MAX_INPUT_LEN - 1) {
                sm.buffer[sm.buffer_pos++] = cmd;
                printf("Added digit: %c\n", cmd);
            } else {
                printf("Buffer full, cannot add more digits\n");
            }
        }
        else {
            State next_state = get_next_state(sm.current_state, cmd);
            if (next_state != sm.current_state) {
                sm.current_state = next_state;
                printf("State changed to: %d\n", sm.current_state);
            }
        }
        
        for (int j = 0; j < 1000000; j++) {
            volatile int x = j * j;
            (void)x;
        }
    }
    
    return 0;
}