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
    uint64_t computation_result;
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
        return;
    }
    
    uint64_t result = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] == '1') {
            uint64_t temp = 1;
            for (int j = 0; j < 16; j++) {
                if (temp > UINT64_MAX / (i + 1)) {
                    temp = UINT64_MAX;
                    break;
                }
                temp = temp * (i + 1);
            }
            if (result > UINT64_MAX - temp) {
                result = UINT64_MAX;
            } else {
                result += temp;
            }
        }
    }
    sm->computation_result = result;
}

static void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->computation_result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static void print_state_info(StateMachine *sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State: IDLE\n");
            break;
        case STATE_READING:
            printf("State: READING (Buffer: %.*s)\n", (int)sm->buffer_pos, sm->buffer);
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING (Result: %lu)\n", sm->computation_result);
            break;
        case STATE_COMPLETE:
            printf("State: COMPLETE\n");
            break;
        case STATE_ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, 0/1=Binary input, E=End input, C=Continue, R=Reset, Q=Quit\n");
    
    int running = 1;
    while (running) {
        print_state_info(&sm);
        printf("Enter command: ");
        
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char cmd = input[0];
        if (!isprint(cmd)) {
            continue;
        }
        
        State next_state = get_next_state(sm.current_state, cmd);
        
        switch (next_state) {
            case STATE_READING:
                if (sm.current_state == STATE_IDLE) {
                    reset_machine(&sm);
                    sm.current_state = STATE_READING;
                } else if (sm.current_state == STATE_READING) {
                    if (cmd == '0' || cmd == '1') {
                        if (sm.buffer_pos < MAX_INPUT_LEN - 1) {
                            sm.buffer[sm.buffer_pos++] = cmd;
                            sm.buffer[sm.buffer_pos] = '\0';
                        } else {
                            next_state = STATE_ERROR;
                        }
                    }
                    sm.current_state = next_state;
                }
                break;
                
            case STATE_PROCESSING:
                if (sm.current_state == STATE_READING) {
                    process_buffer(&sm);
                    sm.current_state = STATE_PROCESSING;
                }
                break;
                
            case STATE_IDLE:
                if (sm.current_state == STATE_PROCESSING || sm.current_state == STATE_ERROR) {
                    reset_machine(&sm);
                    sm.current_state = STATE_IDLE;
                }
                break;
                
            case STATE_COMPLETE: