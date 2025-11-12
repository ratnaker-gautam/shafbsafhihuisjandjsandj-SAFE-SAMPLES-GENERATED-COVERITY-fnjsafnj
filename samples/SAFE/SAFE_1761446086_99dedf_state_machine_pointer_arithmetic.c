//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT 256
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
    char buffer[MAX_INPUT];
    size_t position;
    int result;
} Machine;

void machine_init(Machine *m) {
    m->current = STATE_START;
    m->position = 0;
    m->result = 0;
    memset(m->buffer, 0, MAX_INPUT);
}

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == 'q';
}

State handle_start(Machine *m, char input) {
    if (input == 'q') return STATE_END;
    if (input >= '0' && input <= '9') {
        if (m->position < MAX_INPUT - 1) {
            *(m->buffer + m->position) = input;
            m->position++;
            return STATE_READING;
        }
    }
    return STATE_START;
}

State handle_reading(Machine *m, char input) {
    if (input == '\n' || input == '\0') {
        return STATE_PROCESSING;
    }
    if (m->position < MAX_INPUT - 1 && is_valid_char(input)) {
        *(m->buffer + m->position) = input;
        m->position++;
        return STATE_READING;
    }
    return STATE_READING;
}

State handle_processing(Machine *m) {
    char *ptr = m->buffer;
    int sign = 1;
    long value = 0;
    
    if (*ptr == '+') {
        ptr++;
    } else if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    while (*ptr != '\0') {
        if (*ptr >= '0' && *ptr <= '9') {
            if (value > INT_MAX / 10) {
                m->result = 0;
                return STATE_OUTPUT;
            }
            value = value * 10 + (*ptr - '0');
            if (value > INT_MAX) {
                m->result = 0;
                return STATE_OUTPUT;
            }
        }
        ptr++;
    }
    
    if (sign == -1) {
        value = -value;
        if (value < INT_MIN) {
            m->result = 0;
            return STATE_OUTPUT;
        }
    } else {
        if (value > INT_MAX) {
            m->result = 0;
            return STATE_OUTPUT;
        }
    }
    
    m->result = (int)value;
    return STATE_OUTPUT;
}

State handle_output(Machine *m) {
    printf("Result: %d\n", m->result);
    machine_init(m);
    return STATE_START;
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    printf("Enter numbers (q to quit):\n");
    
    while (m.current != STATE_END) {
        int ch = getchar();
        if (ch == EOF) {
            break;
        }
        char input = (char)ch;
        
        switch (m.current) {
            case STATE_START:
                m.current = handle_start(&m, input);
                break;
            case STATE_READING:
                m.current = handle_reading(&m, input);
                if (m.current == STATE_PROCESSING) {
                    m.current = handle_processing(&m);
                }
                break;
            case STATE_PROCESSING:
                m.current = handle_processing(&m);
                break;
            case STATE_OUTPUT:
                m.current = handle_output(&m);
                break;
            case STATE_END:
                break;
        }
        
        if (m.current == STATE_PROCESSING) {
            m.current = handle_processing(&m);
        }
        if (m.current == STATE_OUTPUT) {
            m.current = handle_output(&m);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}