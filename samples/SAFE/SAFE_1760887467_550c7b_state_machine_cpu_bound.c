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

static Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_ERROR, 'Q', STATE_COMPLETE}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State find_next_state(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

static void process_state(State state, int *counter) {
    switch (state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 1000000; i++) {
                *counter = (*counter + 1) % 1000;
            }
            break;
        case STATE_ERROR:
            *counter = 0;
            break;
        case STATE_COMPLETE:
            break;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    int counter = 0;
    int input_len;
    
    printf("State Machine Simulation\n");
    printf("Commands: S=Start, A=Process, C=Complete, E=Error, R=Reset, Q=Quit\n");
    
    while (current_state != STATE_COMPLETE) {
        printf("State: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
        }
        printf(" | Counter: %d\n", counter);
        printf("> ");
        
        if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }
        
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len != 1) {
            printf("Invalid input length\n");
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        if (strchr("SACERQ", command) == NULL) {
            printf("Invalid command\n");
            continue;
        }
        
        State next_state = find_next_state(current_state, command);
        if (next_state == current_state) {
            printf("Invalid transition\n");
            continue;
        }
        
        current_state = next_state;
        process_state(current_state, &counter);
    }
    
    printf("Final counter value: %d\n", counter);
    return 0;
}