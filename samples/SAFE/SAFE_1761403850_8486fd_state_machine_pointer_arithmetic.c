//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int machine_transition(Machine *m, char input) {
    State next = m->current;
    
    switch (m->current) {
        case STATE_START:
            if (input == 'B' || input == 'b') {
                next = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (m->position < MAX_INPUT - 1) {
                *(m->buffer + m->position) = input;
                m->position++;
                if (input == '\n' || input == 'E') {
                    next = STATE_PROCESSING;
                }
            } else {
                next = STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (m->position > 0) {
                char *ptr = m->buffer;
                int sum = 0;
                while (ptr < m->buffer + m->position) {
                    if (isdigit(*ptr)) {
                        sum += (*ptr - '0');
                    }
                    ptr++;
                }
                m->result = sum;
                next = STATE_OUTPUT;
            } else {
                next = STATE_END;
            }
            break;
            
        case STATE_OUTPUT:
            next = STATE_END;
            break;
            
        case STATE_END:
            break;
    }
    
    m->current = next;
    return (next != STATE_END);
}

void machine_execute(Machine *m) {
    char input;
    int running = 1;
    
    printf("Enter 'B' to begin, then input characters (E to end): ");
    
    while (running && fread(&input, 1, 1, stdin) == 1) {
        running = machine_transition(m, input);
        
        if (m->current == STATE_OUTPUT) {
            printf("Sum of digits: %d\n", m->result);
            machine_init(m);
            printf("Enter 'B' to begin again: ");
        }
    }
}

int main(void) {
    Machine m;
    machine_init(&m);
    machine_execute(&m);
    return 0;
}