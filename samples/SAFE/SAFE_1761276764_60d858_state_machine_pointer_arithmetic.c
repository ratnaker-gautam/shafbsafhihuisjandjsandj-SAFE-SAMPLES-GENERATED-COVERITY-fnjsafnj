//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_START 0
#define STATE_IDENTIFIER 1
#define STATE_NUMBER 2
#define STATE_OPERATOR 3
#define STATE_ERROR 4

typedef struct {
    int current_state;
    char *input;
    size_t position;
    size_t length;
} state_machine_t;

void init_state_machine(state_machine_t *sm, char *input) {
    if (sm == NULL || input == NULL) return;
    sm->current_state = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->length = strlen(input);
}

int process_char(state_machine_t *sm) {
    if (sm == NULL || sm->input == NULL) return STATE_ERROR;
    if (sm->position >= sm->length) return STATE_ERROR;
    
    char current = *(sm->input + sm->position);
    
    switch (sm->current_state) {
        case STATE_START:
            if (isalpha(current)) {
                sm->current_state = STATE_IDENTIFIER;
            } else if (isdigit(current)) {
                sm->current_state = STATE_NUMBER;
            } else if (strchr("+-*/", current) != NULL) {
                sm->current_state = STATE_OPERATOR;
            } else if (isspace(current)) {
                sm->current_state = STATE_START;
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_IDENTIFIER:
            if (!isalnum(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(current)) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_OPERATOR:
            sm->current_state = STATE_ERROR;
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current_state;
}

void process_input(state_machine_t *sm) {
    if (sm == NULL || sm->input == NULL) return;
    
    printf("Processing: %s\n", sm->input);
    
    for (sm->position = 0; sm->position < sm->length; (sm->position)++) {
        int result = process_char(sm);
        
        if (result == STATE_ERROR) {
            printf("Error at position %zu\n", sm->position);
            return;
        }
        
        printf("Position %zu: state %d, char '%c'\n", 
               sm->position, sm->current_state, *(sm->input + sm->position));
    }
    
    printf("Processing completed successfully\n");
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter expression to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    state_machine_t sm;
    init_state_machine(&sm, input_buffer);
    process_input(&sm);
    
    return EXIT_SUCCESS;
}