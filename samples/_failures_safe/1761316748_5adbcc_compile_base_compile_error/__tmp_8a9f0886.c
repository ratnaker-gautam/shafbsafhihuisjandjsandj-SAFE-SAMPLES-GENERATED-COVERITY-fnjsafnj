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
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_ERROR, 'Q', STATE_COMPLETE}
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

static void process_buffer(StateMachine *sm) {
    if (sm->buffer_pos == 0) {
        sm->result = 0;
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] == '1') {
            sum++;
        }
    }
    
    int prime_check = 0;
    if (sum > 1) {
        prime_check = 1;
        for (int i = 2; i * i <= sum; i++) {
            if (sum % i == 0) {
                prime_check = 0;
                break;
            }
        }
    }
    
    sm->result = prime_check;
}

static void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, 0/1=Data, E=End, C=Continue, R=Reset, Q=Quit\n");
    
    int running = 1;
    while (running) {
        printf("State: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | ");
        
        if (sm.current_state == STATE_PROCESSING) {
            printf("Result: %d | ", sm.result);
        }
        
        printf("Enter command: ");
        
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char cmd = toupper(input[0]);
        
        State next_state = get_next_state(sm.current_state, cmd);
        
        switch (cmd) {
            case 'S':
                if (sm.current_state == STATE_IDLE) {
                    reset_machine(&sm);
                    sm.current_state = STATE_READING;
                    printf("Started reading sequence\n");
                }
                break;
                
            case '0':
            case '1':
                if (sm.current_state == STATE_READING) {
                    if (sm.buffer_pos < MAX_INPUT_LEN - 1) {
                        sm.buffer[sm.buffer_pos++] = cmd;
                        printf("Added '%c', buffer size: %zu\n", cmd, sm.buffer_pos);
                    } else {
                        printf("Buffer full!\n");
                        sm.current_state = STATE_ERROR;
                    }
                }
                break;
                
            case 'E':
                if (sm.current_state == STATE_READING) {
                    sm.current_state = STATE_PROCESSING;
                    process_buffer(&sm);
                    printf("Processing complete. Result: %d\n", sm.result);
                }
                break;
                
            case 'C':
                if (sm.current_state == STATE_PROCESSING) {
                    sm.current_state = STATE_IDLE;
                    printf("Returned to idle state\n");
                }
                break;
                
            case 'R':
                if (sm.current_state == STATE_ERROR) {
                    reset_machine(&sm);
                    printf("Machine reset\n");
                }
                break;
                
            case 'Q':
                running = 0;
                printf("Exiting...\n");
                break;
                
            default: