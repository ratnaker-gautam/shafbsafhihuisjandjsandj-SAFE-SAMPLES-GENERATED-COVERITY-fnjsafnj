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
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_len;
    int processed_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_COMPLETE, 'R', STATE_IDLE},
    {STATE_ERROR, 'R', STATE_IDLE}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->processed_count = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

State state_machine_step(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && 
            transitions[i].input_char == input) {
            sm->current_state = transitions[i].next;
            
            if (sm->current_state == STATE_READING && sm->input_len < MAX_INPUT_LEN) {
                sm->input_buffer[sm->input_len++] = input;
            } else if (sm->current_state == STATE_PROCESSING) {
                for (size_t j = 0; j < sm->input_len; j++) {
                    if (sm->input_buffer[j] == 'S') {
                        sm->processed_count++;
                    }
                }
            } else if (sm->current_state == STATE_COMPLETE) {
                sm->processed_count++;
            } else if (sm->current_state == STATE_IDLE) {
                sm->input_len = 0;
                memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
            }
            
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

void process_cpu_intensive_work(int iterations) {
    if (iterations <= 0 || iterations > 1000000) return;
    
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 100; j++) {
            result += (i * j) % 9973;
        }
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_line[MAX_INPUT_LEN + 2];
    int line_count = 0;
    
    printf("Enter state machine commands (S=Start, E=End input, C=Complete, X=Error, R=Reset, Q=Quit):\n");
    
    while (line_count < 100) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        if (input_line[0] == 'Q' && len == 1) {
            break;
        }
        
        for (size_t i = 0; i < len; i++) {
            char cmd = toupper((unsigned char)input_line[i]);
            if (cmd != 'S' && cmd != 'E' && cmd != 'C' && cmd != 'X' && cmd != 'R') {
                continue;
            }
            
            State previous_state = sm.current_state;
            State new_state = state_machine_step(&sm, cmd);
            
            if (new_state == STATE_PROCESSING || new_state == STATE_COMPLETE) {
                process_cpu_intensive_work(10000 + (sm.processed_count * 100));
            }
            
            printf("Command: %c | State: %d -> %d | Processed: %d\n", 
                   cmd, previous_state, new_state, sm.processed_count);
        }
        
        line_count++;
    }
    
    printf("Final processed count: %d\n", sm.processed_count);
    return 0;
}