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
#define MAX_OUTPUT_LEN 200

#define TRANSITION(sm, new_state) do { (sm)->state = (new_state); } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_pos;
    size_t output_pos;
} state_machine_t;

void init_state_machine(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    memset(sm->input_buffer, 0, MAX_INPUT_LEN);
    memset(sm->output_buffer, 0, MAX_OUTPUT_LEN);
}

int handle_idle(state_machine_t *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'S') {
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(state_machine_t *sm, char input) {
    if (sm == NULL) return 0;
    if (!CHECK_BOUNDS(sm->input_pos, MAX_INPUT_LEN - 1)) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (input == 'E') {
        if (sm->input_pos > 0) {
            TRANSITION(sm, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    }
    
    if (IS_VALID_CHAR(input)) {
        sm->input_buffer[sm->input_pos++] = input;
        return 1;
    }
    
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int handle_processing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (!CHECK_BOUNDS(sm->input_pos, MAX_INPUT_LEN) || !CHECK_BOUNDS(sm->output_pos, MAX_OUTPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < sm->input_pos; i++) {
        if (!CHECK_BOUNDS(sm->output_pos, MAX_OUTPUT_LEN - 2)) {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        
        char c = sm->input_buffer[i];
        if (IS_VALID_CHAR(c)) {
            sm->output_buffer[sm->output_pos++] = c;
            sm->output_buffer[sm->output_pos++] = ' ';
        }
    }
    
    if (sm->output_pos > 0 && CHECK_BOUNDS(sm->output_pos - 1, MAX_OUTPUT_LEN)) {
        sm->output_buffer[sm->output_pos - 1] = '\0';
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->output_pos > 0) {
        printf("Processed: %s\n", sm->output_buffer);
    }
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *sm) {
    if (sm == NULL) return 0;
    printf("Error: Invalid state transition or input\n");
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int process_input(state_machine_t *sm, char input) {
    if (sm == NULL) return 0;
    switch (sm->state) {
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
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t sm;
    init_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: S (start), E (end), valid chars (process), Q (quit)\n");
    
    char input;
    int running = 1;
    
    while (running) {
        if (scanf("%c", &input) != 1) {
            break;
        }
        
        if (input == '\n') {
            continue;
        }
        
        if (input == 'Q') {
            running = 0;
            continue;
        }
        
        if (!process_input(&sm, input)) {
            printf("Processing failed\n");
        }
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    printf("Exiting