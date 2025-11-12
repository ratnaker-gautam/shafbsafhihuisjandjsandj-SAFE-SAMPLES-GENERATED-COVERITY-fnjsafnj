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
    uint64_t computation;
    int valid;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_pos = 0;
    sm->computation = 0;
    sm->valid = 1;
}

int is_valid_input(char c) {
    return isdigit(c) || c == ' ' || c == '\n' || c == '\0';
}

State get_next_state(State current, char input, StateMachine *sm) {
    static const Transition transitions[] = {
        {STATE_IDLE, ' ', STATE_IDLE},
        {STATE_IDLE, '\n', STATE_IDLE},
        {STATE_IDLE, '\0', STATE_IDLE},
        {STATE_IDLE, '0', STATE_READING},
        {STATE_IDLE, '1', STATE_READING},
        {STATE_IDLE, '2', STATE_READING},
        {STATE_IDLE, '3', STATE_READING},
        {STATE_IDLE, '4', STATE_READING},
        {STATE_IDLE, '5', STATE_READING},
        {STATE_IDLE, '6', STATE_READING},
        {STATE_IDLE, '7', STATE_READING},
        {STATE_IDLE, '8', STATE_READING},
        {STATE_IDLE, '9', STATE_READING},
        {STATE_READING, ' ', STATE_PROCESSING},
        {STATE_READING, '\n', STATE_PROCESSING},
        {STATE_READING, '\0', STATE_PROCESSING},
        {STATE_READING, '0', STATE_READING},
        {STATE_READING, '1', STATE_READING},
        {STATE_READING, '2', STATE_READING},
        {STATE_READING, '3', STATE_READING},
        {STATE_READING, '4', STATE_READING},
        {STATE_READING, '5', STATE_READING},
        {STATE_READING, '6', STATE_READING},
        {STATE_READING, '7', STATE_READING},
        {STATE_READING, '8', STATE_READING},
        {STATE_READING, '9', STATE_READING},
        {STATE_PROCESSING, ' ', STATE_IDLE},
        {STATE_PROCESSING, '\n', STATE_IDLE},
        {STATE_PROCESSING, '\0', STATE_COMPLETE},
        {STATE_PROCESSING, '0', STATE_READING},
        {STATE_PROCESSING, '1', STATE_READING},
        {STATE_PROCESSING, '2', STATE_READING},
        {STATE_PROCESSING, '3', STATE_READING},
        {STATE_PROCESSING, '4', STATE_READING},
        {STATE_PROCESSING, '5', STATE_READING},
        {STATE_PROCESSING, '6', STATE_READING},
        {STATE_PROCESSING, '7', STATE_READING},
        {STATE_PROCESSING, '8', STATE_READING},
        {STATE_PROCESSING, '9', STATE_READING}
    };
    
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    
    return STATE_ERROR;
}

void process_buffer(StateMachine *sm) {
    if (sm->buffer_pos == 0) return;
    
    char *endptr;
    unsigned long value = strtoul(sm->buffer, &endptr, 10);
    
    if (endptr == sm->buffer || *endptr != '\0') {
        sm->valid = 0;
        return;
    }
    
    if (value > UINT64_MAX - sm->computation) {
        sm->valid = 0;
        return;
    }
    
    for (unsigned long i = 0; i < value; i++) {
        if (sm->computation > UINT64_MAX - 1) {
            sm->valid = 0;
            return;
        }
        sm->computation++;
    }
    
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    printf("Enter numbers separated by spaces (max %d chars): ", MAX_INPUT_LEN - 1);
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if