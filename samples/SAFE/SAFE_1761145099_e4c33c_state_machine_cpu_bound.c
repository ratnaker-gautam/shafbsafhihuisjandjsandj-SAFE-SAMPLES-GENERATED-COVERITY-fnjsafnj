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

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static int process_data(const char* input, int len) {
    int result = 0;
    for (int i = 0; i < len; i++) {
        if (input[i] == 'A') {
            for (int j = 0; j < 1000; j++) {
                result = (result * 31 + input[i]) % 1000000;
            }
        }
    }
    return result;
}

static State get_next_state(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

static void print_state_info(State state) {
    const char* names[] = {"IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"};
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("State: %s\n", names[state]);
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    int input_len = 0;
    int processed_result = 0;
    
    printf("State Machine Simulation\n");
    printf("Commands: S=Start, A=Append, C=Complete, X=Error, R=Reset, Q=Quit\n");
    
    while (1) {
        print_state_info(current_state);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[--input_len] = '\0';
        }
        
        if (input_len == 0) {
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        
        if (command == 'Q') {
            break;
        }
        
        State next_state = get_next_state(current_state, command);
        
        switch (command) {
            case 'S':
                if (current_state == STATE_IDLE) {
                    input_len = 0;
                    processed_result = 0;
                    printf("Started reading data\n");
                }
                break;
                
            case 'A':
                if (current_state == STATE_READING && input_len > 1) {
                    for (int i = 1; i < input_len; i++) {
                        if (!isalnum(input_buffer[i])) {
                            next_state = STATE_ERROR;
                            break;
                        }
                    }
                    if (next_state != STATE_ERROR) {
                        printf("Appended %d characters\n", input_len - 1);
                    }
                }
                break;
                
            case 'C':
                if (current_state == STATE_PROCESSING) {
                    processed_result = process_data(input_buffer, input_len);
                    printf("Processing complete. Result: %d\n", processed_result);
                }
                break;
                
            case 'X':
                printf("Error condition triggered\n");
                break;
                
            case 'R':
                if (current_state == STATE_ERROR || current_state == STATE_COMPLETE) {
                    input_len = 0;
                    processed_result = 0;
                    printf("System reset\n");
                }
                break;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            printf("Invalid transition or input error\n");
        }
    }
    
    printf("Simulation ended\n");
    return 0;
}