//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    if (sm == NULL) return;
    sm->current = STATE_START;
    sm->pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '=';
}

State handle_start(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == '=') return STATE_END;
    if (is_valid_char(input)) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == '=') {
        if (sm->pos > 0) return STATE_PROCESSING;
        return STATE_END;
    }
    if (is_valid_char(input) && sm->pos < MAX_INPUT_LEN - 1) {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
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
    
    if (sign == 1 && value < 0) return STATE_END;
    if (sign == -1) {
        if (value == INT_MAX && value > 0) return STATE_END;
        value = -value;
    }
    
    sm->result = value;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    printf("Result: %d\n", sm->result);
    return STATE_END;
}

void process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
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

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter arithmetic expression (numbers and +- followed by =): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        return EXIT_FAILURE;
    }
    
    state_machine_init(&sm);
    process_input(&sm, input);
    
    return EXIT_SUCCESS;
}