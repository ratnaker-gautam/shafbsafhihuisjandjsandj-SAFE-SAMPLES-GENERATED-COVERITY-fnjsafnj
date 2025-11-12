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
            if (input == 'B') {
                next = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (input == 'E') {
                next = STATE_PROCESSING;
            } else if (m->pos < MAX_INPUT_LEN - 1) {
                *(m->buffer + m->pos) = input;
                m->pos++;
            }
            break;
            
        case STATE_PROCESSING:
            if (m->pos > 0) {
                char *ptr = m->buffer;
                char *end = m->buffer + m->pos;
                int sum = 0;
                
                while (ptr < end) {
                    if (*ptr >= '0' && *ptr <= '9') {
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

int main(void) {
    Machine m;
    machine_init(&m);
    
    printf("Enter sequence (start with B, end with E): ");
    
    int running = 1;
    while (running) {
        int c = getchar();
        if (c == EOF || c == '\n') {
            break;
        }
        
        if (c < 0 || c > 127) {
            continue;
        }
        
        running = machine_transition(&m, (char)c);
        
        if (m.current == STATE_OUTPUT) {
            printf("Sum of digits: %d\n", m.result);
            machine_transition(&m, ' ');
        }
    }
    
    if (m.current != STATE_END) {
        printf("Incomplete sequence\n");
    }
    
    return 0;
}