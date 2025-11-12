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
    if (input == 'q') return STATE_END;
    if (input >= '0' && input <= '9') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_READING;
        }
    } else if (input == '+' || input == '-') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_PROCESSING;
        }
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, char input) {
    if (input == 'q') return STATE_END;
    if (input >= '0' && input <= '9') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_PROCESSING;
        }
    } else if (input == '=') {
        return STATE_OUTPUT;
    }
    return STATE_PROCESSING;
}

State handle_output(StateMachine *sm) {
    char *ptr = sm->buffer;
    int total = 0;
    int sign = 1;
    int current = 0;

    while (*ptr != '\0' && ptr < sm->buffer + MAX_INPUT_LEN) {
        if (*ptr >= '0' && *ptr <= '9') {
            current = current * 10 + (*ptr - '0');
        } else if (*ptr == '+') {
            total += sign * current;
            current = 0;
            sign = 1;
        } else if (*ptr == '-') {
            total += sign * current;
            current = 0;
            sign = -1;
        }
        ptr++;
    }
    total += sign * current;
    sm->result = total;
    printf("Result: %d\n", sm->result);
    return STATE_START;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    char input;

    printf("Enter numbers with + or - operators, then = to calculate (q to quit):\n");

    while (sm.current != STATE_END) {
        if (scanf("%c", &input) != 1) {
            break;
        }

        if (input == '\n') {
            continue;
        }

        if (!is_valid_char(input)) {
            continue;
        }

        switch (sm.current) {
            case STATE_START:
                sm.current = handle_start(&sm, input);
                break;
            case STATE_READING:
                sm.current = handle_reading(&sm, input);
                break;
            case STATE_PROCESSING:
                sm.current = handle_processing(&sm, input);
                break;
            case STATE_OUTPUT:
                sm.current = handle_output(&sm);
                state_machine_init(&sm);
                if (input == 'q') {
                    sm.current = STATE_END;
                }
                break;
            case STATE_END:
                break;
        }
    }

    printf("Goodbye!\n");
    return 0;
}