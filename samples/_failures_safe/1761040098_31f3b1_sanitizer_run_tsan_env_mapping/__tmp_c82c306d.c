//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_COMPLETE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_binary_string(const char* str, size_t len) {
    if (len == 0 || len > 63) return -1;
    
    int result = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1') return -1;
        result = (result << 1) | (str[i] - '0');
        if (result < 0) return -1;
    }
    return result;
}

void state_machine_init(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_process(StateMachine* sm, char input) {
    if (sm->current_state == STATE_ERROR || sm->current_state == STATE_COMPLETE) {
        return -1;
    }
    
    State next_state = get_next_state(sm->current_state, input);
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
            
        case STATE_READING:
            if (input == '0' || input == '1') {
                if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (input == 'E') {
                sm->buffer[sm->buffer_pos] = '\0';
            }
            break;
            
        case STATE_PROCESSING:
            sm->result = process_binary_string(sm->buffer, sm->buffer_pos);
            if (sm->result < 0) {
                next_state = STATE_ERROR;
            }
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
    
    char input_buffer[MAX_INPUT_LEN * 2];
    
    printf("Enter sequence (S[01]*E): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (state_machine_process(&sm, input_buffer[i]) != 0) {
            break;
        }
    }
    
    if (sm.current_state == STATE_COMPLETE) {
        printf("Result: %d\n", sm.result);
        return 0;
    } else {
        fprintf(stderr, "Invalid sequence or processing error\n");
        return 1;
    }
}