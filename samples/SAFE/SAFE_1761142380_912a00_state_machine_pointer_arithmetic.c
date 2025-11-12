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
    if (m == NULL) return 0;
    
    State next = m->current;
    
    switch (m->current) {
        case STATE_START:
            if (input == 'B' || input == 'E') {
                next = STATE_READING;
                if (m->pos < MAX_INPUT_LEN - 1) {
                    *(m->buffer + m->pos) = input;
                    m->pos++;
                }
            }
            break;
            
        case STATE_READING:
            if (m->pos < MAX_INPUT_LEN - 1) {
                if (input == 'X') {
                    next = STATE_PROCESSING;
                } else if (input >= 'A' && input <= 'Z') {
                    *(m->buffer + m->pos) = input;
                    m->pos++;
                } else {
                    next = STATE_END;
                }
            } else {
                next = STATE_END;
            }
            break;
            
        case STATE_PROCESSING:
            if (input == 'C') {
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
    return next != STATE_END;
}

void machine_process(Machine *m) {
    if (m == NULL || m->current != STATE_OUTPUT) return;
    
    char *ptr = m->buffer;
    char *end = m->buffer + MAX_INPUT_LEN;
    while (ptr < end && *ptr != '\0') {
        if (*ptr >= 'A' && *ptr <= 'Z') {
            m->result += (int)(*ptr - 'A' + 1);
        }
        ptr++;
    }
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    printf("Enter sequence (B/E followed by letters, X, then C): ");
    
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (!machine_transition(&m, (char)ch)) {
            break;
        }
    }
    
    if (m.current == STATE_OUTPUT) {
        machine_process(&m);
        printf("Result: %d\n", m.result);
        
        printf("Buffer contents: ");
        char *ptr = m.buffer;
        char *end = m.buffer + MAX_INPUT_LEN;
        while (ptr < end && *ptr != '\0') {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    } else {
        printf("Invalid sequence or state machine error\n");
    }
    
    return 0;
}