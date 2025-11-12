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
    sm->pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
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
        if (value > INT_MAX / 10) {
            sm->result = 0;
            return STATE_END;
        }
        int digit = *ptr - '0';
        if (value > (INT_MAX - digit) / 10) {
            sm->result = 0;
            return STATE_END;
        }
        value = value * 10 + digit;
        ptr++;
    }
    
    if (sign == 1 && value > INT_MAX) {
        sm->result = 0;
        return STATE_END;
    }
    if (sign == -1 && value > -(INT_MIN + 1)) {
        sm->result = 0;
        return STATE_END;
    }
    
    sm->result = sign * value;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    printf("Result: %d\n", sm->result);
    return STATE_END;
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
        ptr++;
    }
    
    if (sm->current == STATE_READING && sm->pos > 0) {
        sm->current = STATE_PROCESSING;
    }
    
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    
    if (sm->current == STATE_OUTPUT) {
        sm->current = handle_output(sm);
    }
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter calculation (e.g., +123= or -456=): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    state_machine_init(&sm);
    process_input(&sm, input);
    
    return 0;
}