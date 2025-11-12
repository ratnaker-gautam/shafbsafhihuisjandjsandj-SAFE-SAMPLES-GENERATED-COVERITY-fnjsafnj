//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
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
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int result;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->current = STATE_START;
    sm->pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == 'q';
}

State handle_start(StateMachine *sm, char input) {
    if (input == 'q') return STATE_END;
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (input == '\n' || input == '\0') {
        if (sm->pos > 0) return STATE_PROCESSING;
        return STATE_START;
    }
    if (sm->pos >= MAX_INPUT_LEN - 1) return STATE_PROCESSING;
    if (is_valid_char(input)) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm) {
    char *endptr = NULL;
    long val = strtol(sm->buffer, &endptr, 10);
    if (endptr != sm->buffer && *endptr == '\0' && val >= -1000 && val <= 1000) {
        sm->result = (int)val;
        return STATE_OUTPUT;
    }
    return STATE_START;
}

State handle_output(StateMachine *sm) {
    printf("Processed value: %d\n", sm->result);
    return STATE_START;
}

void process_input(StateMachine *sm, const char *input) {
    const char *ptr = input;
    while (*ptr != '\0' && sm->current != STATE_END) {
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, *ptr);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, *ptr);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                break;
            case STATE_END:
                return;
        }
        if (sm->current != STATE_PROCESSING && sm->current != STATE_OUTPUT) {
            ptr++;
        }
    }
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_OUTPUT) {
        sm->current = handle_output(sm);
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_line[MAX_INPUT_LEN];
    
    printf("Enter numbers (q to quit):\n");
    
    while (sm.current != STATE_END) {
        if (fgets(input_line, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }
        
        process_input(&sm, input_line);
        
        if (sm.current == STATE_START) {
            state_machine_init(&sm);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}