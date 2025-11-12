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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    uint64_t computation_result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_COMPLETE},
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_ERROR}
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

static uint64_t cpu_intensive_computation(const char* data, size_t len) {
    uint64_t result = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t val = (uint64_t)data[i];
        for (int j = 0; j < 1000; j++) {
            val = (val * 1103515245 + 12345) & 0x7FFFFFFF;
        }
        result ^= val;
        result = (result << 1) | (result >> 63);
    }
    return result;
}

static void state_machine_run(StateMachine* sm, const char* input) {
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN) {
        sm->current_state = STATE_ERROR;
        return;
    }

    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->computation_result = 0;

    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        State next_state = get_next_state(sm->current_state, c);
        
        switch (sm->current_state) {
            case STATE_IDLE:
                if (next_state == STATE_READING) {
                    sm->buffer[0] = '\0';
                    sm->buffer_pos = 0;
                }
                break;
                
            case STATE_READING:
                if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buffer_pos++] = c;
                    sm->buffer[sm->buffer_pos] = '\0';
                } else {
                    next_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                sm->computation_result = cpu_intensive_computation(sm->buffer, sm->buffer_pos);
                break;
                
            default:
                break;
        }
        
        sm->current_state = next_state;
        if (sm->current_state == STATE_ERROR || sm->current_state == STATE_COMPLETE) {
            break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    StateMachine sm;
    
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine_run(&sm, input);
    
    switch (sm.current_state) {
        case STATE_COMPLETE:
            printf("Processing complete. Result: %lu\n", sm.computation_result);
            break;
        case STATE_ERROR:
            printf("Error occurred during processing\n");
            break;
        default:
            printf("Unexpected termination in state: %d\n", sm.current_state);
            break;
    }
    
    return 0;
}