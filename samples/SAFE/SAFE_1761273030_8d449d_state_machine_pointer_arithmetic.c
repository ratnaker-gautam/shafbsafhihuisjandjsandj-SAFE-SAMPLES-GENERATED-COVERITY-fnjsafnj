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
} Machine;

void machine_init(Machine *m) {
    m->current = STATE_START;
    m->pos = 0;
    m->result = 0;
    memset(m->buffer, 0, MAX_INPUT_LEN);
}

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '=';
}

State handle_start(Machine *m, char input) {
    if (input == '=') {
        return STATE_END;
    }
    if (is_valid_char(input)) {
        *(m->buffer + m->pos) = input;
        m->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(Machine *m, char input) {
    if (input == '=') {
        return STATE_PROCESSING;
    }
    if (is_valid_char(input) && m->pos < MAX_INPUT_LEN - 1) {
        *(m->buffer + m->pos) = input;
        m->pos++;
        return STATE_READING;
    }
    return STATE_READING;
}

State handle_processing(Machine *m) {
    char *ptr = m->buffer;
    int sum = 0;
    int sign = 1;
    int temp = 0;
    
    while (*ptr != '\0' && ptr < m->buffer + MAX_INPUT_LEN) {
        if (*ptr == '+') {
            sum += sign * temp;
            temp = 0;
            sign = 1;
        } else if (*ptr == '-') {
            sum += sign * temp;
            temp = 0;
            sign = -1;
        } else if (*ptr >= '0' && *ptr <= '9') {
            temp = temp * 10 + (*ptr - '0');
        }
        ptr++;
    }
    sum += sign * temp;
    
    m->result = sum;
    return STATE_OUTPUT;
}

State handle_output(Machine *m) {
    printf("Result: %d\n", m->result);
    machine_init(m);
    return STATE_START;
}

void process_input(Machine *m, const char *input) {
    const char *ptr = input;
    
    while (*ptr != '\0' && m->current != STATE_END) {
        switch (m->current) {
            case STATE_START:
                m->current = handle_start(m, *ptr);
                break;
            case STATE_READING:
                m->current = handle_reading(m, *ptr);
                break;
            case STATE_PROCESSING:
                m->current = handle_processing(m);
                break;
            case STATE_OUTPUT:
                m->current = handle_output(m);
                break;
            case STATE_END:
                return;
        }
        ptr++;
    }
    
    if (m->current == STATE_PROCESSING) {
        m->current = handle_processing(m);
    }
    if (m->current == STATE_OUTPUT) {
        m->current = handle_output(m);
    }
}

int main() {
    Machine m;
    char input[MAX_INPUT_LEN * 2];
    
    machine_init(&m);
    
    printf("Enter expressions (numbers with +-), end with '=': ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(&m, input);
    
    return 0;
}