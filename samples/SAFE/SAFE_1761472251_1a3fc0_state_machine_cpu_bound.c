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
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_ERROR, 'Q', STATE_COMPLETE}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

static void process_buffer(StateMachine *sm) {
    if (sm->buffer_pos == 0) {
        sm->result = 0;
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] == '1') {
            sum++;
        }
    }
    
    for (int i = 0; i < 1000000; i++) {
        sum = (sum * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    
    sm->result = sum % 1000;
    sm->buffer_pos = 0;
}

static void state_machine_run(StateMachine *sm) {
    char input_line[MAX_INPUT_LEN + 2];
    
    printf("State Machine Started\n");
    printf("Commands: S(start), 0/1(data), E(end), C(continue), R(reset), Q(quit)\n");
    
    while (sm->current_state != STATE_COMPLETE) {
        printf("State: %d > ", sm->current_state);
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        for (size_t i = 0; i < len; i++) {
            char input_char = toupper((unsigned char)input_line[i]);
            State next_state = get_next_state(sm->current_state, input_char);
            
            switch (next_state) {
                case STATE_READING:
                    if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                        sm->buffer[sm->buffer_pos++] = input_char;
                    }
                    break;
                    
                case STATE_PROCESSING:
                    process_buffer(sm);
                    printf("Processing complete. Result: %d\n", sm->result);
                    break;
                    
                case STATE_IDLE:
                    sm->buffer_pos = 0;
                    sm->result = 0;
                    break;
                    
                case STATE_ERROR:
                    printf("Invalid transition from state %d with input '%c'\n", 
                           sm->current_state, input_char);
                    break;
                    
                default:
                    break;
            }
            
            sm->current_state = next_state;
            
            if (sm->current_state == STATE_COMPLETE) {
                break;
            }
        }
    }
    
    printf("State Machine Terminated\n");
}

int main(void) {
    StateMachine sm;
    
    sm.current_state = STATE_IDLE;
    sm.buffer_pos = 0;
    sm.result = 0;
    
    memset(sm.buffer, 0, sizeof(sm.buffer));
    
    state_machine_run(&sm);
    
    return 0;
}