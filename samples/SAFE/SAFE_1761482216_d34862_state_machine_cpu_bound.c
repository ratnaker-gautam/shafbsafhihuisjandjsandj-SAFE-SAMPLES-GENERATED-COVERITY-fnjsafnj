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
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_COMPLETE},
    {STATE_COMPLETE, '\0', STATE_IDLE}
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

static int process_binary_string(const char* str, size_t len) {
    if (len == 0 || len > 63) return 0;
    int result = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '1') {
            if (result > (INT_MAX - 1) / 2) return 0;
            result = result * 2 + 1;
        } else if (str[i] == '0') {
            if (result > INT_MAX / 2) return 0;
            result = result * 2;
        } else {
            return 0;
        }
    }
    return result;
}

static void reset_machine(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static int run_state_machine(StateMachine* sm, const char* input) {
    reset_machine(sm);
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) return 0;

    for (size_t i = 0; i < input_len; i++) {
        char current_char = input[i];
        State next_state = get_next_state(sm->current_state, current_char);
        
        if (next_state == STATE_ERROR) return 0;
        
        switch (sm->current_state) {
            case STATE_READING:
                if (current_char == '0' || current_char == '1') {
                    if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                        sm->buffer[sm->buffer_pos++] = current_char;
                    } else {
                        return 0;
                    }
                }
                break;
            case STATE_PROCESSING:
                if (sm->buffer_pos > 0) {
                    sm->result = process_binary_string(sm->buffer, sm->buffer_pos);
                }
                break;
            default:
                break;
        }
        
        sm->current_state = next_state;
        if (sm->current_state == STATE_COMPLETE) break;
    }

    if (sm->current_state == STATE_COMPLETE && sm->buffer_pos > 0) {
        return sm->result;
    }
    return 0;
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    printf("Enter binary sequence (format: S[binary digits]E): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int result = run_state_machine(&sm, input);
    if (result > 0) {
        printf("Decimal value: %d\n", result);
    } else {
        printf("Invalid input format or overflow\n");
    }
    
    return 0;
}