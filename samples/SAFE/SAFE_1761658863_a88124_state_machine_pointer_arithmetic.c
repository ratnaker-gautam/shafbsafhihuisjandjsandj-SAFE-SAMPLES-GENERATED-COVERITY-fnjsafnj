//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

typedef struct {
    int current_state;
    char *input_ptr;
    char *output_ptr;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_INPUT_LEN];
} StateMachine;

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_ptr = sm->input_buffer;
    sm->output_ptr = sm->output_buffer;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
    memset(sm->output_buffer, 0, sizeof(sm->output_buffer));
}

int read_input(StateMachine *sm) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(sm->input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(sm->input_buffer);
    if (len > 0 && *(sm->input_ptr + len - 1) == '\n') {
        *(sm->input_ptr + len - 1) = '\0';
    }
    
    return 0;
}

void process_character(StateMachine *sm, char c) {
    if (isalpha(c)) {
        if (isupper(c)) {
            *(sm->output_ptr) = tolower(c);
        } else {
            *(sm->output_ptr) = toupper(c);
        }
        sm->output_ptr++;
    } else if (isdigit(c)) {
        *(sm->output_ptr) = '*';
        sm->output_ptr++;
    } else {
        *(sm->output_ptr) = c;
        sm->output_ptr++;
    }
}

int run_state_machine(StateMachine *sm) {
    while (sm->current_state != STATE_DONE) {
        switch (sm->current_state) {
            case STATE_IDLE:
                if (read_input(sm) != 0) {
                    return -1;
                }
                sm->current_state = STATE_READING;
                break;
                
            case STATE_READING:
                if (*(sm->input_ptr) == '\0') {
                    sm->current_state = STATE_DONE;
                } else {
                    sm->current_state = STATE_PROCESSING;
                }
                break;
                
            case STATE_PROCESSING:
                if (*(sm->input_ptr) != '\0') {
                    process_character(sm, *(sm->input_ptr));
                    sm->input_ptr++;
                    sm->current_state = STATE_READING;
                } else {
                    sm->current_state = STATE_DONE;
                }
                break;
                
            default:
                return -1;
        }
    }
    
    return 0;
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    printf("Text Processor State Machine\n");
    printf("Converts letters case and replaces digits with '*'\n");
    
    if (run_state_machine(&sm) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", sm.input_buffer);
    printf("Processed: %s\n", sm.output_buffer);
    
    return EXIT_SUCCESS;
}