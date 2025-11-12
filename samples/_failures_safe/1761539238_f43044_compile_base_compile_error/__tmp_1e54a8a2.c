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
    int error_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, 'A', STATE_READING},
    {STATE_READING, 'B', STATE_READING},
    {STATE_READING, 'C', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_PROCESSING, 'P', STATE_IDLE},
    {STATE_PROCESSING, 'R', STATE_READING},
    {STATE_ERROR, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_input_buffer(StateMachine *sm) {
    if (sm->input_len == 0) {
        return;
    }
    
    int valid_count = 0;
    for (size_t i = 0; i < sm->input_len; i++) {
        if (sm->input_buffer[i] == 'A' || sm->input_buffer[i] == 'B' || sm->input_buffer[i] == 'C') {
            valid_count++;
        }
    }
    
    if (valid_count > 0) {
        sm->processed_count += valid_count;
    }
    
    sm->input_len = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

void handle_state_action(StateMachine *sm, char input) {
    switch (sm->current_state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            if (sm->input_len < MAX_INPUT_LEN) {
                sm->input_buffer[sm->input_len++] = input;
            }
            break;
        case STATE_PROCESSING:
            process_input_buffer(sm);
            break;
        case STATE_COMPLETE:
            break;
        case STATE_ERROR:
            sm->error_count++;
            break;
    }
}

int is_valid_input(char c) {
    return c == 'S' || c == 'Q' || c == 'A' || c == 'B' || c == 'C' || 
           c == 'E' || c == 'P' || c == 'R';
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, Q=Quit, A/B/C=Data, E=End input, P=Process, R=Reset\n");
    printf("Enter command sequence: ");
    
    char input_line[MAX_INPUT_LEN * 2 + 1];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 1;
    }
    
    size_t line_len = strlen(input_line);
    if (line_len > 0 && input_line[line_len - 1] == '\n') {
        input_line[line_len - 1] = '\0';
        line_len--;
    }
    
    for (size_t i = 0; i < line_len; i++) {
        char current_char = toupper(input_line[i]);
        
        if (!is_valid_input(current_char)) {
            sm.current_state = STATE_ERROR;
            handle_state_action(&sm, current_char);
            continue;
        }
        
        State next_state = get_next_state(sm.current_state, current_char);
        handle_state_action(&sm, current_char);
        sm.current_state = next_state;
        
        if (sm.current_state == STATE_COMPLETE) {
            break;
        }
    }
    
    if (sm.current_state == STATE_READING || sm.current_state == STATE_PROCESSING) {
        process_input_buffer(&sm);
    }
    
    printf("Final state: ");
    switch (sm.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\n");
    printf("Processed items: %d\n", sm.processed_count);
    printf