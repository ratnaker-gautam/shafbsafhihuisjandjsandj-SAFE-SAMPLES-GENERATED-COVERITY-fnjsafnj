//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 50

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    int buffer_pos;
    int input_count;
} StateMachine;

void init_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->input_count = 0;
}

int handle_idle(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'S') {
        printf("Starting data collection\n");
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'E') {
        if (sm->buffer_pos > 0) {
            TRANSITION(sm, STATE_PROCESSING);
            return 1;
        } else {
            printf("Error: No data to process\n");
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    } else if (CHECK_BOUNDS(sm->buffer_pos, MAX_BUFFER-1) && input >= '0' && input <= '9') {
        sm->buffer[sm->buffer_pos++] = input;
        sm->input_count++;
        return 1;
    } else {
        printf("Error: Invalid input or buffer full\n");
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) {
        printf("Error: Empty buffer\n");
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    int sum = 0;
    for (int i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= '0' && sm->buffer[i] <= '9') {
            sum += (sm->buffer[i] - '0');
        }
    }
    
    printf("Processed %d digits, sum: %d\n", sm->buffer_pos, sum);
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Writing results...\n");
    for (int i = 0; i < sm->buffer_pos; i++) {
        if (CHECK_BOUNDS(i, MAX_BUFFER)) {
            printf("Digit %d: %c\n", i+1, sm->buffer[i]);
        }
    }
    printf("Total inputs processed: %d\n", sm->input_count);
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 1;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Error state - resetting\n");
    TRANSITION(sm, STATE_IDLE);
    init_machine(sm);
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    switch(sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input);
        case STATE_READING:
            return handle_reading(sm, input);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            printf("Unknown state\n");
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    StateMachine sm;
    init_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: S=Start, 0-9=Digits, E=End, Q=Quit\n");
    
    while (running) {
        printf("Current state: %d > ", sm.state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error\n");
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input, "Q") == 0) {
            printf("Quitting\n");
            break;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (!process_input(&sm, input[i])) {
                break;
            }
        }
    }
    
    return 0;
}