//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
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
    if (sm == NULL) return;
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
    if (input == 'q') return STATE_END;
    if (input >= '0' && input <= '9') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_READING;
        }
        return STATE_PROCESSING;
    }
    if (input == '+' || input == '-') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_PROCESSING;
        }
        return STATE_PROCESSING;
    }
    return STATE_PROCESSING;
}

State handle_processing(StateMachine *sm) {
    if (sm->pos == 0) return STATE_START;
    
    char *endptr = NULL;
    long value = strtol(sm->buffer, &endptr, 10);
    
    if (endptr != sm->buffer && *endptr == '\0') {
        if (value >= -1000 && value <= 1000) {
            sm->result = (int)value;
            return STATE_OUTPUT;
        }
    }
    
    return STATE_START;
}

State handle_output(StateMachine *sm) {
    printf("Processed number: %d\n", sm->result);
    sm->pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_START;
}

void process_input(StateMachine *sm, char input) {
    if (sm == NULL || !is_valid_char(input)) return;
    
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
    
    printf("Enter numbers or 'q' to quit:\n");
    
    char input;
    while (sm.current != STATE_END) {
        int read_result = scanf("%c", &input);
        if (read_result != 1) {
            break;
        }
        
        if (input == '\n') {
            if (sm.current == STATE_READING || sm.current == STATE_PROCESSING) {
                process_input(&sm, '+');
            }
            continue;
        }
        
        process_input(&sm, input);
    }
    
    printf("Goodbye!\n");
    return 0;
}