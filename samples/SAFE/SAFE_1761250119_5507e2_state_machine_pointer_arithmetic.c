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
    if (m == NULL) return;
    m->current = STATE_START;
    m->pos = 0;
    m->result = 0;
    memset(m->buffer, 0, MAX_INPUT_LEN);
}

int machine_transition(Machine *m, char input) {
    if (m == NULL) return -1;
    
    switch (m->current) {
        case STATE_START:
            if (input == 'B') {
                m->current = STATE_READING;
                m->pos = 0;
            }
            break;
            
        case STATE_READING:
            if (input == 'E') {
                m->current = STATE_PROCESSING;
            } else if (m->pos < MAX_INPUT_LEN - 1) {
                *(m->buffer + m->pos) = input;
                m->pos++;
                *(m->buffer + m->pos) = '\0';
            }
            break;
            
        case STATE_PROCESSING:
            if (m->pos > 0) {
                char *ptr = m->buffer;
                while (*ptr != '\0') {
                    if (*ptr >= '0' && *ptr <= '9') {
                        m->result += (*ptr - '0');
                    }
                    ptr++;
                }
                m->current = STATE_OUTPUT;
            } else {
                m->current = STATE_END;
            }
            break;
            
        case STATE_OUTPUT:
            printf("Sum of digits: %d\n", m->result);
            m->current = STATE_END;
            break;
            
        case STATE_END:
            break;
    }
    
    return 0;
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && *ptr != '\n') {
        if (machine_transition(&m, *ptr) != 0) {
            break;
        }
        ptr++;
        
        if (m.current == STATE_END) {
            break;
        }
    }
    
    if (m.current != STATE_END && m.current != STATE_OUTPUT) {
        machine_transition(&m, 'E');
    }
    
    return 0;
}