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
    return (c >= '0' && c <= '9') || c == ' ' || c == '\n' || c == '\0';
}

State handle_start(StateMachine *sm, char input) {
    if (input == '\0') {
        return STATE_END;
    }
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (input >= '0' && input <= '9') {
        if (sm->pos >= MAX_INPUT_LEN - 1) {
            return STATE_END;
        }
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    if (input == ' ' || input == '\n' || input == '\0') {
        *(sm->buffer + sm->pos) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    char *endptr;
    long value = strtol(sm->buffer, &endptr, 10);
    if (endptr == sm->buffer || *endptr != '\0') {
        return STATE_END;
    }
    if (value < -1000000 || value > 1000000) {
        return STATE_END;
    }
    sm->result += (int)value;
    sm->pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm, char input) {
    printf("Current sum: %d\n", sm->result);
    if (input == '\0') {
        return STATE_END;
    }
    return STATE_START;
}

void process_input(StateMachine *sm, const char *input) {
    const char *ptr = input;
    while (sm->current != STATE_END && *ptr != '\0') {
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
                break;
            case STATE_OUTPUT:
                sm->current = handle_output(sm, *ptr);
                break;
            case STATE_END:
                break;
        }
        if (sm->current != STATE_PROCESSING) {
            ptr++;
        }
    }
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_OUTPUT) {
        handle_output(sm, '\0');
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    size_t len = strlen(input);
    if (len >= sizeof(input) - 1) {
        return 1;
    }
    input[len] = '\0';
    
    process_input(&sm, input);
    
    return 0;
}