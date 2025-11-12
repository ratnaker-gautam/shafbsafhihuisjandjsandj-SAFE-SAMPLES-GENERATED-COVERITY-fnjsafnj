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
    STATE_PROCESSING,
    STATE_VALIDATING,
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
    {STATE_IDLE, 'S', STATE_PROCESSING},
    {STATE_PROCESSING, 'D', STATE_PROCESSING},
    {STATE_PROCESSING, 'V', STATE_VALIDATING},
    {STATE_VALIDATING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_VALIDATING, 'E', STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_input(StateMachine *sm, const char *input) {
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        return -1;
    }

    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (!isalpha(c)) {
            return -1;
        }
        c = toupper(c);

        State next_state = get_next_state(sm->current_state, c);
        if (next_state == STATE_ERROR) {
            sm->current_state = STATE_ERROR;
            return -1;
        }

        if (sm->current_state == STATE_PROCESSING && c == 'D') {
            if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                sm->buffer[sm->buffer_pos++] = c;
            } else {
                sm->current_state = STATE_ERROR;
                return -1;
            }
        }

        sm->current_state = next_state;

        if (sm->current_state == STATE_COMPLETE) {
            sm->result = (int)sm->buffer_pos;
            return 0;
        }
    }

    return 1;
}

void simulate_workload(int iterations) {
    volatile int counter = 0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            counter += i * j;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    StateMachine sm;
    
    init_state_machine(&sm);
    
    printf("Enter state sequence (S=Start, D=Data, V=Validate, C=Complete, E=Error): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int result = process_input(&sm, input);
    
    simulate_workload(10000);
    
    switch (sm.current_state) {
        case STATE_IDLE:
            printf("Result: IDLE (no processing)\n");
            break;
        case STATE_PROCESSING:
            printf("Result: PROCESSING (incomplete)\n");
            break;
        case STATE_VALIDATING:
            printf("Result: VALIDATING (pending completion)\n");
            break;
        case STATE_COMPLETE:
            printf("Result: COMPLETE (processed %d data items)\n", sm.result);
            break;
        case STATE_ERROR:
            printf("Result: ERROR (invalid sequence)\n");
            break;
    }
    
    return result == 0 ? 0 : 1;
}