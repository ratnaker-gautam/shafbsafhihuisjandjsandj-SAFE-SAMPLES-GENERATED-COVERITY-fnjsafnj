//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
} Machine;

void machine_init(Machine *m) {
    m->current = STATE_START;
    m->pos = 0;
    m->result = 0;
    memset(m->buffer, 0, MAX_INPUT_LEN);
}

int machine_transition(Machine *m, char input) {
    State next = m->current;
    int valid = 1;

    switch (m->current) {
        case STATE_START:
            if (input == 'B' || input == 'E') {
                next = STATE_READING;
                *(m->buffer + m->pos) = input;
                m->pos++;
            } else {
                valid = 0;
            }
            break;
        case STATE_READING:
            if (input >= '0' && input <= '9') {
                *(m->buffer + m->pos) = input;
                m->pos++;
                if (m->pos >= MAX_INPUT_LEN - 1) {
                    next = STATE_PROCESSING;
                }
            } else if (input == ';') {
                next = STATE_PROCESSING;
            } else {
                valid = 0;
            }
            break;
        case STATE_PROCESSING:
            if (input == 'C') {
                next = STATE_OUTPUT;
            } else {
                valid = 0;
            }
            break;
        case STATE_OUTPUT:
            if (input == 'D') {
                next = STATE_END;
            } else {
                valid = 0;
            }
            break;
        case STATE_END:
            valid = 0;
            break;
    }

    if (valid) {
        m->current = next;
    }
    return valid;
}

void machine_process(Machine *m) {
    if (m->current != STATE_PROCESSING) {
        return;
    }

    char *ptr = m->buffer + 1;
    int sum = 0;
    while (*ptr && *ptr != ';' && (ptr - m->buffer) < (int)m->pos) {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    m->result = sum;
    m->current = STATE_OUTPUT;
}

void machine_output(Machine *m) {
    if (m->current == STATE_OUTPUT) {
        printf("Result: %d\n", m->result);
        m->current = STATE_END;
    }
}

int main(void) {
    Machine m;
    machine_init(&m);

    printf("Enter sequence (format: B<digits>;CD): ");
    fflush(stdout);

    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    char *ptr = input;
    while (*ptr && *ptr != '\n') {
        if (!machine_transition(&m, *ptr)) {
            printf("Invalid input sequence\n");
            return EXIT_FAILURE;
        }
        if (m.current == STATE_PROCESSING) {
            machine_process(&m);
        }
        if (m.current == STATE_OUTPUT) {
            machine_output(&m);
        }
        ptr++;
    }

    if (m.current != STATE_END) {
        printf("Incomplete sequence\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}