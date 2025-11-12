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

Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'A', STATE_READING},
    {STATE_READING, 'B', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'D', STATE_ERROR}
};

const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State process_transition(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_data(const char* data, int len) {
    volatile int result = 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 1000; j++) {
            result += (int)data[i] * j;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    int input_len = 0;
    
    printf("Enter sequence (S to start, A/B to read, E to process, C/D to complete/error): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i < input_len; i++) {
        if (!isalpha(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        input[i] = toupper(input[i]);
    }
    
    for (int i = 0; i < input_len; i++) {
        current_state = process_transition(current_state, input[i]);
        if (current_state == STATE_ERROR) {
            printf("State machine entered error state at position %d\n", i);
            break;
        }
        
        if (current_state == STATE_READING) {
            printf("Reading data...\n");
        } else if (current_state == STATE_PROCESSING) {
            printf("Processing data...\n");
            process_data(input, i + 1);
        } else if (current_state == STATE_COMPLETE) {
            printf("Processing complete!\n");
            break;
        }
    }
    
    if (current_state != STATE_COMPLETE && current_state != STATE_ERROR) {
        printf("State machine did not reach terminal state\n");
    }
    
    return 0;
}