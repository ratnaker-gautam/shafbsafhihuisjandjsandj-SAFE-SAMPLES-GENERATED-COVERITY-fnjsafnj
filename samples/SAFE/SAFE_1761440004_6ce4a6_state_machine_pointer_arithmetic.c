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
    if (sm == NULL || input == NULL) return;
    sm->current = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->output = malloc(MAX_INPUT * 2);
    if (sm->output != NULL) {
        memset(sm->output, 0, MAX_INPUT * 2);
    }
    sm->output_pos = 0;
}

void state_machine_cleanup(StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->output != NULL) {
        free(sm->output);
        sm->output = NULL;
    }
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '.' || c == ',';
}

State handle_start(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    if (*(sm->input + sm->position) != '\0') {
        return STATE_READING;
    }
    return STATE_END;
}

State handle_reading(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    if (sm->position >= MAX_INPUT) return STATE_END;
    char current = *(sm->input + sm->position);
    if (current == '\0') {
        return STATE_END;
    }
    if (!is_valid_char(current)) {
        sm->position++;
        return STATE_READING;
    }
    return STATE_PROCESSING;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL || sm->output == NULL) return STATE_END;
    if (sm->output_pos >= (MAX_INPUT * 2 - 2)) return STATE_OUTPUT;
    char current = *(sm->input + sm->position);
    if (isalpha(current)) {
        *(sm->output + sm->output_pos) = toupper(current);
        sm->output_pos++;
    } else if (isdigit(current)) {
        *(sm->output + sm->output_pos) = current;
        sm->output_pos++;
    } else {
        *(sm->output + sm->output_pos) = current;
        sm->output_pos++;
    }
    sm->position++;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    if (sm->output_pos >= (MAX_INPUT * 2 - 2)) return STATE_END;
    *(sm->output + sm->output_pos) = '|';
    sm->output_pos++;
    return STATE_READING;
}

void run_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    while (sm->current != STATE_END) {
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                break;
            case STATE_END:
                return;
            default:
                sm->current = STATE_END;
                break;
        }
    }
}

int main(void) {
    char user_input[MAX_INPUT];
    
    printf("Enter text to process: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (strlen(user_input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    StateMachine sm;
    state_machine_init(&sm, user_input);
    
    if (sm.output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    run_state_machine(&sm);
    
    printf("Processed output: %s\n", sm.output);
    
    state_machine_cleanup(&sm);
    return 0;
}