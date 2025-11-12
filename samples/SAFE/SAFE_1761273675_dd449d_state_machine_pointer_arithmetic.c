//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    sm->current = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->pos = 0;
    sm->result = 0;
}

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '=';
}

State handle_start(StateMachine *sm, char input) {
    if (input == '=') {
        return STATE_END;
    }
    if (is_valid_char(input)) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (input == '=') {
        return STATE_PROCESSING;
    }
    if (is_valid_char(input) && sm->pos < MAX_INPUT_LEN - 1) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm) {
    char *ptr = sm->buffer;
    int sign = 1;
    int value = 0;
    
    if (*ptr == '+' || *ptr == '-') {
        sign = (*ptr == '-') ? -1 : 1;
        ptr++;
    }
    
    while (*ptr != '\0' && *ptr >= '0' && *ptr <= '9') {
        int digit = *ptr - '0';
        if (value > (INT_MAX - digit) / 10) {
            return STATE_END;
        }
        value = value * 10 + digit;
        ptr++;
    }
    
    sm->result = value * sign;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    printf("Result: %d\n", sm->result);
    state_machine_init(sm);
    return STATE_START;
}

void process_input(StateMachine *sm, const char *input) {
    const char *ptr = input;
    while (*ptr != '\0') {
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
            default:
                sm->current = STATE_START;
                break;
        }
        ptr++;
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
    char input[MAX_INPUT_LEN * 2];
    
    state_machine_init(&sm);
    
    printf("Enter expressions (numbers with +- prefix, end with =):\n");
    printf("Type 'quit' to exit.\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strncmp(input, "quit", 4) == 0) {
            break;
        }
        
        char *newline = strchr(input, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }
        
        if (strlen(input) >= sizeof(input) - 1) {
            printf("Input too long\n");
            continue;
        }
        
        process_input(&sm, input);
        
        if (sm.current == STATE_END) {
            break;
        }
    }
    
    return 0;
}