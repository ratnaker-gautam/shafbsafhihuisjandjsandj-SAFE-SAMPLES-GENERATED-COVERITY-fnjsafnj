//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
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
    int processed_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, '\0', STATE_DONE},
    {STATE_READING, '\n', STATE_ERROR},
    {STATE_PROCESSING, '\n', STATE_ERROR},
    {STATE_IDLE, '\n', STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    sm->processed_count = 0;
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

int process_character(StateMachine *sm, char c) {
    if (sm->input_pos >= MAX_INPUT_LEN - 1) {
        return -1;
    }
    
    State next_state = get_next_state(sm->current_state, c);
    if (next_state == STATE_ERROR) {
        return -1;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (c == 'S') {
                sm->input_buffer[sm->input_pos++] = c;
            }
            break;
        case STATE_READING:
            if (c == 'E' || c == '\0') {
                sm->input_buffer[sm->input_pos] = '\0';
            } else if (isalnum((unsigned char)c)) {
                sm->input_buffer[sm->input_pos++] = c;
            } else {
                return -1;
            }
            break;
        case STATE_PROCESSING:
            if (c == '\0') {
                for (size_t i = 0; i < sm->input_pos && sm->output_pos < MAX_INPUT_LEN - 1; i++) {
                    char processed = sm->input_buffer[i];
                    if (isalpha((unsigned char)processed)) {
                        processed = (char)(((processed - 'A' + 1) % 26) + 'A');
                    }
                    sm->output_buffer[sm->output_pos++] = processed;
                }
                sm->output_buffer[sm->output_pos] = '\0';
                sm->processed_count++;
            }
            break;
        case STATE_OUTPUT:
            if (c == '\0') {
                printf("Processed: %s\n", sm->output_buffer);
            }
            break;
        default:
            return -1;
    }
    
    sm->current_state = next_state;
    return 0;
}

void cpu_intensive_transform(char *str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        for (int j = 0; j < 1000; j++) {
            str[i] = (char)((str[i] * 17 + 23) % 128);
            if (str[i] < 32) str[i] += 32;
        }
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char test_sequence[] = "STARTDATA123END";
    
    for (size_t i = 0; i < strlen(test_sequence); i++) {
        if (process_character(&sm, test_sequence[i]) != 0) {
            fprintf(stderr, "State machine error at position %zu\n", i);
            return EXIT_FAILURE;
        }
    }
    
    if (process_character(&sm, '\0') != 0) {
        fprintf(stderr, "Final state transition error\n");
        return EXIT_FAILURE;
    }
    
    cpu_intensive_transform(sm.output_buffer);
    
    printf("Final result: %s\n", sm.output_buffer);
    printf("Total processed: %d\n", sm.processed_count);
    
    return EXIT_SUCCESS;
}