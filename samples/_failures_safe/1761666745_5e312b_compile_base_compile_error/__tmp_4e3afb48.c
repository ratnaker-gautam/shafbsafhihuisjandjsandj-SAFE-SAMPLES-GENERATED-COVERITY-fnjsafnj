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
    size_t input_len;
    size_t position;
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, ' ', STATE_IDLE},
    {STATE_IDLE, '\n', STATE_IDLE},
    {STATE_IDLE, '\t', STATE_IDLE},
    {STATE_IDLE, '0', STATE_READING},
    {STATE_IDLE, '1', STATE_READING},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, ' ', STATE_PROCESSING},
    {STATE_READING, '\n', STATE_PROCESSING},
    {STATE_READING, '\t', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, ' ', STATE_PROCESSING},
    {STATE_PROCESSING, '\n', STATE_PROCESSING},
    {STATE_PROCESSING, '\t', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, ' ', STATE_IDLE},
    {STATE_OUTPUT, '\n', STATE_IDLE},
    {STATE_OUTPUT, '\t', STATE_IDLE},
    {STATE_OUTPUT, '0', STATE_READING},
    {STATE_OUTPUT, '1', STATE_READING},
    {STATE_OUTPUT, '\0', STATE_DONE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

static int binary_to_decimal(const char* binary, size_t len) {
    int decimal = 0;
    for (size_t i = 0; i < len; i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            return -1;
        }
        if (decimal > INT_MAX / 2) {
            return -1;
        }
        decimal = decimal * 2 + (binary[i] - '0');
        if (decimal < 0) {
            return -1;
        }
    }
    return decimal;
}

static void process_binary_number(StateMachine* sm) {
    if (sm->input_len > 0) {
        sm->result = binary_to_decimal(sm->input_buffer, sm->input_len);
        sm->input_len = 0;
    }
}

static void state_machine_init(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->position = 0;
    sm->result = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

static int state_machine_step(StateMachine* sm, char input) {
    State next_state = get_next_state(sm->current_state, input);
    
    if (next_state == STATE_ERROR) {
        return -1;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == '0' || input == '1') {
                if (sm->input_len < MAX_INPUT_LEN - 1) {
                    sm->input_buffer[sm->input_len++] = input;
                } else {
                    return -1;
                }
            }
            break;
            
        case STATE_READING:
            if (input == '0' || input == '1') {
                if (sm->input_len < MAX_INPUT_LEN - 1) {
                    sm->input_buffer[sm->input_len++] = input;
                } else {
                    return -1;
                }
            } else if (input == ' ' || input == '\n' || input == '\t' || input == '\0') {
                process_binary_number(sm);
            }
            break;
            
        case STATE_PROCESSING:
            if (input == '\0') {
                process_binary_number(sm);
            }
            break;
            
        case STATE_OUTPUT:
            printf("%d ", sm->result);
            sm->result = 0;
            break;
            
        default:
            break;
    }
    
    sm->current_state = next_state;
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_line[MAX_INPUT_LEN + 2];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_length = strlen(input_line);
    if (input_length > 0 && input_line[input_length