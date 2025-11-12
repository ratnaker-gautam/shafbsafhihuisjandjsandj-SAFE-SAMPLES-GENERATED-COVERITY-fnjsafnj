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

int machine_transition(Machine *m, char input) {
    State next = m->current;
    
    switch (m->current) {
        case STATE_START:
            if (input == 'B' || input == 'E') {
                next = STATE_READING;
                *(m->buffer + m->pos) = input;
                m->pos++;
            }
            break;
            
        case STATE_READING:
            if (input == ' ') {
                next = STATE_PROCESSING;
            } else if (m->pos < MAX_INPUT_LEN - 1) {
                *(m->buffer + m->pos) = input;
                m->pos++;
            }
            break;
            
        case STATE_PROCESSING:
            if (*(m->buffer) == 'B') {
                char *ptr = m->buffer + 1;
                while (ptr < m->buffer + m->pos) {
                    if (*ptr >= '0' && *ptr <= '9') {
                        m->result += (*ptr - '0');
                    }
                    ptr++;
                }
            } else if (*(m->buffer) == 'E') {
                m->result = -1;
            }
            next = STATE_OUTPUT;
            break;
            
        case STATE_OUTPUT:
            next = STATE_END;
            break;
            
        default:
            break;
    }
    
    m->current = next;
    return (next != STATE_END);
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && *ptr != '\n') {
        if (!machine_transition(&m, *ptr)) {
            break;
        }
        ptr++;
    }
    
    if (m.current == STATE_OUTPUT) {
        machine_transition(&m, ' ');
    }
    
    if (m.result >= 0) {
        printf("Result: %d\n", m.result);
    } else {
        printf("End signal received\n");
    }
    
    return EXIT_SUCCESS;
}