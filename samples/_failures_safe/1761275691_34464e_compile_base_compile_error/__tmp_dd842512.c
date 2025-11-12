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
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_pos;
    char output_buffer[MAX_INPUT_LEN + 1];
    size_t output_pos;
    int processed_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, 'A', STATE_READING},
    {STATE_READING, 'B', STATE_READING},
    {STATE_READING, 'C', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, '\0', STATE_DONE},
    {STATE_ERROR, '\0', STATE_DONE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    sm->processed_count = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
    memset(sm->output_buffer, 0, sizeof(sm->output_buffer));
}

int process_character(StateMachine *sm, char c) {
    if (sm->input_pos >= MAX_INPUT_LEN) {
        sm->current_state = STATE_ERROR;
        return 0;
    }
    
    sm->input_buffer[sm->input_pos++] = c;
    State next_state = get_next_state(sm->current_state, c);
    
    switch (next_state) {
        case STATE_READING:
            sm->current_state = STATE_READING;
            break;
        case STATE_PROCESSING:
            sm->current_state = STATE_PROCESSING;
            for (size_t i = 0; i < sm->input_pos - 1; i++) {
                if (sm->input_buffer[i] == 'A' || sm->input_buffer[i] == 'B' || sm->input_buffer[i] == 'C') {
                    if (sm->output_pos < MAX_INPUT_LEN) {
                        sm->output_buffer[sm->output_pos++] = sm->input_buffer[i];
                        sm->processed_count++;
                    }
                }
            }
            sm->current_state = STATE_OUTPUT;
            break;
        case STATE_OUTPUT:
            sm->current_state = STATE_OUTPUT;
            break;
        case STATE_ERROR:
            sm->current_state = STATE_ERROR;
            return 0;
        case STATE_DONE:
            sm->current_state = STATE_DONE;
            return 0;
        default:
            sm->current_state = STATE_ERROR;
            return 0;
    }
    return 1;
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    printf("Enter sequence (start with S, end with E, use A/B/C between): ");
    fflush(stdout);
    
    char input_line[MAX_INPUT_LEN + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input_line[i];
        if (!isupper(c) && c != 'S' && c != 'E') {
            fprintf(stderr, "Invalid character: %c\n", c);
            return 1;
        }
        
        if (!process_character(&sm, c)) {
            break;
        }
    }
    
    if (sm.current_state != STATE_DONE && sm.current_state != STATE_OUTPUT) {
        if (!process_character(&sm, '\0')) {
            fprintf(stderr, "State machine error\n");
            return 1;
        }
    }
    
    if (sm.current_state == STATE_DONE || sm.current_state == STATE_OUTPUT) {
        printf("Processed %d valid characters: %s\n", sm.processed_count, sm.output_buffer);
        return 0;
    } else {
        fprintf(stderr, "