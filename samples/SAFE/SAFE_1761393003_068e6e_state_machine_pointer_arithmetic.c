//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

typedef struct {
    int current_state;
    char *input_ptr;
    char *output_ptr;
    char input_buffer[MAX_INPUT];
    char output_buffer[MAX_INPUT];
} StateMachine;

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_ptr = sm->input_buffer;
    sm->output_ptr = sm->output_buffer;
    memset(sm->input_buffer, 0, MAX_INPUT);
    memset(sm->output_buffer, 0, MAX_INPUT);
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT - 1) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

void process_character(StateMachine *sm, char c) {
    switch (sm->current_state) {
        case STATE_IDLE:
            if (isalpha(c)) {
                *sm->output_ptr = toupper(c);
                sm->output_ptr++;
                sm->current_state = STATE_READING;
            }
            break;
        case STATE_READING:
            if (isalpha(c)) {
                *sm->output_ptr = tolower(c);
                sm->output_ptr++;
            } else if (c == ' ') {
                *sm->output_ptr = '_';
                sm->output_ptr++;
                sm->current_state = STATE_PROCESSING;
            } else {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (isalpha(c)) {
                *sm->output_ptr = toupper(c);
                sm->output_ptr++;
                sm->current_state = STATE_READING;
            } else if (c == ' ') {
                *sm->output_ptr = '_';
                sm->output_ptr++;
            } else {
                sm->current_state = STATE_IDLE;
            }
            break;
        default:
            break;
    }
}

void run_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->output_ptr = sm->output_buffer;
    
    char *ptr = sm->input_ptr;
    while (*ptr != '\0' && (ptr - sm->input_ptr) < MAX_INPUT - 1) {
        process_character(sm, *ptr);
        ptr++;
    }
    
    *sm->output_ptr = '\0';
    sm->current_state = STATE_DONE;
}

int main() {
    StateMachine sm;
    init_state_machine(&sm);
    
    printf("Enter text to process: ");
    if (fgets(sm.input_buffer, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(sm.input_buffer);
    if (len > 0 && sm.input_buffer[len - 1] == '\n') {
        sm.input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(sm.input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    run_state_machine(&sm);
    
    if (sm.current_state == STATE_DONE) {
        printf("Processed output: %s\n", sm.output_buffer);
    } else {
        fprintf(stderr, "Processing failed\n");
        return 1;
    }
    
    return 0;
}