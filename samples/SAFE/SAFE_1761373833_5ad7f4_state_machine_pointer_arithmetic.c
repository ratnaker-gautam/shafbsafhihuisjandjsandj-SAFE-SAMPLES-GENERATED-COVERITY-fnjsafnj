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
    if (input == '\n' || input == '\r') {
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
    if (sm->pos == 0) return STATE_START;
    
    char *endptr = NULL;
    long value = strtol(sm->buffer, &endptr, 10);
    
    if (endptr != sm->buffer + sm->pos) return STATE_START;
    if (value < -1000 || value > 1000) return STATE_START;
    
    sm->result = (int)value;
    sm->pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    printf("Processed value: %d\n", sm->result);
    return STATE_START;
}

void process_input(StateMachine *sm, char input) {
    switch (sm->current) {
        case STATE_START:
            sm->current = handle_start(sm, input);
            break;
        case STATE_READING:
            sm->current = handle_reading(sm, input);
            break;
        case STATE_PROCESSING:
            sm->current = handle_processing(sm);
            break;
        case STATE_OUTPUT:
            sm->current = handle_output(sm);
            break;
        case STATE_END:
            break;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter numbers or 'q' to quit:\n");
    
    while (sm.current != STATE_END) {
        int c = getchar();
        if (c == EOF) break;
        
        process_input(&sm, (char)c);
        
        if (sm.current == STATE_PROCESSING) {
            process_input(&sm, '\0');
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}