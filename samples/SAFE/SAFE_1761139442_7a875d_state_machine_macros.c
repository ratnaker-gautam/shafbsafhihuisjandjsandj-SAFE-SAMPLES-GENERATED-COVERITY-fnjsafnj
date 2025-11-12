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
    sm->state = STATE_IDLE;
    sm->input_len = 0;
    sm->output_len = 0;
    memset(sm->input_buffer, 0, MAX_INPUT_LEN);
    memset(sm->output_buffer, 0, MAX_OUTPUT_LEN);
}

int process_input(state_machine_t *sm, const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    
    if (sm->state != STATE_IDLE && sm->state != STATE_READING) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
        if (sm->input_len >= MAX_INPUT_LEN - 1) return 0;
        sm->input_buffer[sm->input_len++] = input[i];
    }
    sm->input_buffer[sm->input_len] = '\0';
    
    TRANSITION(sm, STATE_READING);
    return 1;
}

int process_data(state_machine_t *sm) {
    if (sm->state != STATE_READING) return 0;
    if (sm->input_len == 0) return 0;
    
    sm->output_len = 0;
    for (size_t i = 0; i < sm->input_len; i++) {
        if (sm->output_len >= MAX_OUTPUT_LEN - 1) return 0;
        
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
    
    TRANSITION(sm, STATE_PROCESSING);
    return 1;
}

int write_output(state_machine_t *sm) {
    if (sm->state != STATE_PROCESSING) return 0;
    if (sm->output_len == 0) return 0;
    
    printf("Processed: %s\n", sm->output_buffer);
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

void reset_state_machine(state_machine_t *sm) {
    init_state_machine(sm);
    TRANSITION(sm, STATE_IDLE);
}

int main(void) {
    state_machine_t sm;
    init_state_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    int choice;
    
    while (1) {
        printf("\nState Machine Menu:\n");
        printf("1. Enter input\n");
        printf("2. Process data\n");
        printf("3. Write output\n");
        printf("4. Reset\n");
        printf("5. Exit\n");
        printf("Current state: %d\n", sm.state);
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter text: ");
                if (scanf("%99s", input) != 1) {
                    printf("Input error\n");
                    break;
                }
                if (!process_input(&sm, input)) {
                    printf("Input processing failed\n");
                    TRANSITION(&sm, STATE_ERROR);
                }
                break;
                
            case 2:
                if (!process_data(&sm)) {
                    printf("Data processing failed\n");
                    TRANSITION(&sm, STATE_ERROR);
                }
                break;
                
            case 3:
                if (!write_output(&sm)) {
                    printf("Output writing failed\n");
                    TRANSITION(&sm, STATE_ERROR);
                }
                break;
                
            case 4:
                reset_state_machine(&sm);
                printf("State machine reset\n");
                break;
                
            case 5:
                return 0;
                
            default:
                printf("Invalid choice\n");
                break;
        }
        
        if (sm.state == STATE_ERROR) {
            printf("Error state reached. Resetting...\n");
            reset_state_machine(&sm);
        }
    }
    
    return 0;
}