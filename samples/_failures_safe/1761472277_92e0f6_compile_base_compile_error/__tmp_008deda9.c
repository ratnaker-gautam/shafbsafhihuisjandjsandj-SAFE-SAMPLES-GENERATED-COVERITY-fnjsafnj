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

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_len;
    size_t output_len;
} state_machine_t;

void init_state_machine(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->input_len = 0;
    sm->output_len = 0;
    memset(sm->input_buffer, 0, MAX_INPUT_LEN);
    memset(sm->output_buffer, 0, MAX_OUTPUT_LEN);
}

int process_input(state_machine_t *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t len = strlen(input);
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    
    if (sm->state != STATE_IDLE) return 0;
    
    strncpy(sm->input_buffer, input, MAX_INPUT_LEN - 1);
    sm->input_buffer[MAX_INPUT_LEN - 1] = '\0';
    sm->input_len = strlen(sm->input_buffer);
    
    TRANSITION(sm, STATE_READING);
    return 1;
}

int validate_input(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->state != STATE_READING) return 0;
    
    for (size_t i = 0; i < sm->input_len; i++) {
        if (!IS_VALID_CHAR(sm->input_buffer[i])) {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    }
    
    TRANSITION(sm, STATE_PROCESSING);
    return 1;
}

int transform_data(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->state != STATE_PROCESSING) return 0;
    
    sm->output_len = 0;
    for (size_t i = 0; i < sm->input_len; i++) {
        if (sm->output_len >= MAX_OUTPUT_LEN - 2) break;
        
        char c = sm->input_buffer[i];
        if (c >= 'a' && c <= 'z') {
            sm->output_buffer[sm->output_len++] = c - 32;
        } else if (c >= 'A' && c <= 'Z') {
            sm->output_buffer[sm->output_len++] = c + 32;
        } else {
            sm->output_buffer[sm->output_len++] = c;
        }
    }
    sm->output_buffer[sm->output_len] = '\0';
    
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int output_result(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->state != STATE_WRITING) return 0;
    
    if (sm->output_len > 0) {
        printf("Processed output: %s\n", sm->output_buffer);
    } else {
        printf("No output generated\n");
    }
    
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->state != STATE_ERROR) return 0;
    
    printf("Error: Invalid input detected\n");
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

void run_state_machine(state_machine_t *sm) {
    if (sm == NULL) return;
    
    int processed = 0;
    
    do {
        processed = 0;
        
        switch (sm->state) {
            case STATE_READING:
                processed = validate_input(sm);
                break;
            case STATE_PROCESSING:
                processed = transform_data(sm);
                break;
            case STATE_WRITING:
                processed = output_result(sm);
                break;
            case STATE_ERROR:
                processed = handle_error(sm);
                break;
            default:
                break;
        }
    } while (processed);
}

int main(void) {
    state_machine_t sm;
    char user_input[MAX_INPUT_LEN];
    
    init_state_machine(&sm);
    
    printf("Enter text to process (or 'quit' to exit): ");
    
    while (fgets(user_input, MAX_INPUT_LEN, stdin) != NULL) {
        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }
        
        if (strcmp(user_input, "quit") == 0) {
            break;
        }
        
        if (process_input(&sm, user_input)) {
            run