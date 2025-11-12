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
    size_t input_pos;
    int processed_count;
    int error_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'P', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_IDLE},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'R', STATE_READING},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_COMPLETE, 'R', STATE_READING},
    {STATE_ERROR, 'R', STATE_READING}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_pos = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

State state_machine_transition(StateMachine *sm, char input) {
    if (!isprint(input)) {
        return sm->current_state;
    }
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && 
            transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    
    return sm->current_state;
}

void state_machine_process(StateMachine *sm, const char *input) {
    if (input == NULL) {
        return;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        State new_state = state_machine_transition(sm, input[i]);
        
        if (sm->input_pos < MAX_INPUT_LEN) {
            sm->input_buffer[sm->input_pos++] = input[i];
        }
        
        if (new_state != sm->current_state) {
            switch (new_state) {
                case STATE_PROCESSING:
                    for (int j = 0; j < 1000; j++) {
                        volatile int computation = j * j + j;
                        (void)computation;
                    }
                    sm->processed_count++;
                    break;
                case STATE_COMPLETE:
                    sm->processed_count++;
                    break;
                case STATE_ERROR:
                    sm->error_count++;
                    break;
                default:
                    break;
            }
            sm->current_state = new_state;
        }
    }
}

void print_state_info(const StateMachine *sm) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Input Buffer: %s\n", sm->input_buffer);
    printf("Processed Count: %d\n", sm->processed_count);
    printf("Error Count: %d\n", sm->error_count);
    printf("---\n");
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    const char *test_inputs[] = {
        "SPC",
        "SPRSPXRC",
        "SE",
        "SPRSPCR",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    };
    size_t num_tests = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        printf("Processing: %s\n", test_inputs[i]);
        state_machine_process(&sm, test_inputs[i]);
        print_state_info(&sm);
    }
    
    return 0;
}