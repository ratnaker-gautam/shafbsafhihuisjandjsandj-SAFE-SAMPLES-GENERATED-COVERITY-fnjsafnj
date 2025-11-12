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

int is_valid_input(char c) {
    return (c >= '0' && c <= '9') || c == 'q';
}

State handle_start(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == 'q') return STATE_END;
    if (is_valid_input(input)) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == 'q') return STATE_END;
    if (is_valid_input(input)) {
        if (sm->pos >= MAX_INPUT_LEN - 1) return STATE_PROCESSING;
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_PROCESSING;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    if (sm->pos == 0) return STATE_OUTPUT;
    
    char *ptr = sm->buffer;
    int sum = 0;
    while (ptr < sm->buffer + sm->pos) {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    sm->result = sum;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    printf("Sum of digits: %d\n", sm->result);
    return STATE_START;
}

void process_state_machine(StateMachine *sm, char input) {
    if (sm == NULL) return;
    
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

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter digits (q to quit): ");
    fflush(stdout);
    
    char input;
    while (sm.current != STATE_END) {
        if (scanf("%c", &input) != 1) {
            break;
        }
        
        if (input == '\n') {
            if (sm.current == STATE_READING || sm.current == STATE_START) {
                sm.current = STATE_PROCESSING;
            }
            continue;
        }
        
        process_state_machine(&sm, input);
        
        if (sm.current == STATE_OUTPUT) {
            state_machine_init(&sm);
            printf("Enter digits (q to quit): ");
            fflush(stdout);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}