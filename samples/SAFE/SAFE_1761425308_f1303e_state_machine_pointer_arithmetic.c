//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef struct {
    State current;
    char *input;
    size_t position;
    char *output;
    size_t output_pos;
} StateMachine;

void state_machine_init(StateMachine *sm, char *input) {
    sm->current = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->output = malloc(MAX_INPUT * 2);
    sm->output_pos = 0;
}

void state_machine_cleanup(StateMachine *sm) {
    free(sm->output);
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(*(input + i))) return 0;
    }
    return 1;
}

void process_character(StateMachine *sm, char c) {
    if (sm->output_pos >= MAX_INPUT * 2 - 2) return;
    
    if (isalpha(c)) {
        *(sm->output + sm->output_pos) = toupper(c);
        sm->output_pos++;
    } else if (isdigit(c)) {
        *(sm->output + sm->output_pos) = '[';
        sm->output_pos++;
        *(sm->output + sm->output_pos) = c;
        sm->output_pos++;
        *(sm->output + sm->output_pos) = ']';
        sm->output_pos++;
    } else {
        *(sm->output + sm->output_pos) = c;
        sm->output_pos++;
    }
}

int run_state_machine(StateMachine *sm) {
    if (sm == NULL || sm->input == NULL || sm->output == NULL) return 0;
    
    while (sm->current != STATE_END) {
        switch (sm->current) {
            case STATE_START:
                if (*(sm->input + sm->position) == '\0') {
                    sm->current = STATE_END;
                } else {
                    sm->current = STATE_READING;
                }
                break;
                
            case STATE_READING:
                if (*(sm->input + sm->position) == '\0') {
                    sm->current = STATE_END;
                } else {
                    sm->current = STATE_PROCESSING;
                }
                break;
                
            case STATE_PROCESSING:
                process_character(sm, *(sm->input + sm->position));
                sm->position++;
                sm->current = STATE_OUTPUT;
                break;
                
            case STATE_OUTPUT:
                if (*(sm->input + sm->position) == '\0') {
                    sm->current = STATE_END;
                } else {
                    sm->current = STATE_READING;
                }
                break;
                
            case STATE_END:
                break;
        }
    }
    
    *(sm->output + sm->output_pos) = '\0';
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT];
    
    printf("Enter text to process: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
    }
    
    if (!is_valid_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    StateMachine sm;
    state_machine_init(&sm, input_buffer);
    
    if (!run_state_machine(&sm)) {
        fprintf(stderr, "State machine error\n");
        state_machine_cleanup(&sm);
        return 1;
    }
    
    printf("Processed output: %s\n", sm.output);
    state_machine_cleanup(&sm);
    
    return 0;
}