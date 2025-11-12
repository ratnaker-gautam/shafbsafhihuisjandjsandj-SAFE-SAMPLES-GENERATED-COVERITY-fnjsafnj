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
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_pos;
    char output_buffer[MAX_INPUT_LEN];
    size_t output_pos;
    int error_code;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, 'P', STATE_OUTPUT},
    {STATE_PROCESSING, '\0', STATE_ERROR},
    {STATE_OUTPUT, 'C', STATE_DONE},
    {STATE_OUTPUT, '\0', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    sm->error_code = 0;
    memset(sm->input_buffer, 0, MAX_INPUT_LEN);
    memset(sm->output_buffer, 0, MAX_INPUT_LEN);
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return -1;
    
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) return -1;
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input[i];
        
        if (!isalpha(current_char) && current_char != '\0') {
            sm->error_code = 2;
            sm->current_state = STATE_ERROR;
            return -1;
        }
        
        State next_state = get_next_state(sm->current_state, current_char);
        
        if (next_state == STATE_ERROR) {
            sm->error_code = 1;
            sm->current_state = STATE_ERROR;
            return -1;
        }
        
        sm->current_state = next_state;
        
        if (sm->current_state == STATE_READING && sm->input_pos < MAX_INPUT_LEN - 1) {
            sm->input_buffer[sm->input_pos++] = current_char;
        }
        
        if (sm->current_state == STATE_PROCESSING) {
            for (size_t j = 0; j < sm->input_pos && sm->output_pos < MAX_INPUT_LEN - 1; j++) {
                sm->output_buffer[sm->output_pos++] = toupper(sm->input_buffer[j]);
            }
        }
        
        if (sm->current_state == STATE_DONE) {
            break;
        }
    }
    
    return 0;
}

void print_state_info(const StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_OUTPUT: printf("OUTPUT"); break;
        case STATE_ERROR: printf("ERROR"); break;
        case STATE_DONE: printf("DONE"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\n");
    
    if (sm->input_pos > 0) {
        printf("Input buffer: %s\n", sm->input_buffer);
    }
    
    if (sm->output_pos > 0) {
        printf("Output buffer: %s\n", sm->output_buffer);
    }
    
    if (sm->error_code != 0) {
        printf("Error code: %d\n", sm->error_code);
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char user_input[MAX_INPUT_LEN];
    
    printf("Enter state sequence (S=Start, E=End input, P=Process, C=Complete, R=Reset): ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(user_input, MAX_INPUT_LEN);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0 || len >= MAX_INPUT_LEN