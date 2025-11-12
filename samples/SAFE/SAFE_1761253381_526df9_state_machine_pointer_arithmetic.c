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
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == ' ';
}

State handle_start(StateMachine *sm, char input) {
    if (input == ' ' || input == '\t') {
        return STATE_START;
    }
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_reading(StateMachine *sm, char input) {
    if (input >= '0' && input <= '9') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_READING;
        }
        return STATE_END;
    }
    if (input == ' ' || input == '\t' || input == '\n' || input == '\0') {
        *(sm->buffer + sm->pos) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    char *endptr = NULL;
    long val = strtol(sm->buffer, &endptr, 10);
    if (endptr != sm->buffer + sm->pos) {
        return STATE_END;
    }
    if (val > INT32_MAX || val < INT32_MIN) {
        return STATE_END;
    }
    sm->result = (int)val;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    printf("Parsed number: %d\n", sm->result);
    return STATE_END;
}

void process_input(StateMachine *sm, const char *input) {
    const char *ptr = input;
    while (*ptr != '\0' && sm->current != STATE_END) {
        if (!is_valid_char(*ptr)) {
            sm->current = STATE_END;
            break;
        }
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, *ptr);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, *ptr);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                continue;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                continue;
            case STATE_END:
                break;
        }
        ptr++;
    }
    if (sm->current == STATE_READING && sm->pos > 0) {
        *(sm->buffer + sm->pos) = '\0';
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
    char input[MAX_INPUT_LEN + 1] = {0};
    printf("Enter a number: ");
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    StateMachine sm;
    state_machine_init(&sm);
    process_input(&sm, input);
    if (sm.current == STATE_END && sm.pos == 0) {
        fprintf(stderr, "Failed to parse number\n");
        return 1;
    }
    return 0;
}