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
    size_t length;
    int processed_count;
} Machine;

void machine_init(Machine *m) {
    if (m == NULL) return;
    m->current = STATE_START;
    memset(m->buffer, 0, MAX_INPUT_LEN);
    m->length = 0;
    m->processed_count = 0;
}

int machine_transition(Machine *m, char input) {
    if (m == NULL) return 0;
    
    switch (m->current) {
        case STATE_START:
            if (input == 'B') {
                m->current = STATE_READING;
                m->length = 0;
            }
            break;
            
        case STATE_READING:
            if (input == 'E') {
                if (m->length > 0) {
                    m->current = STATE_PROCESSING;
                } else {
                    m->current = STATE_START;
                }
            } else if (m->length < MAX_INPUT_LEN - 1) {
                *(m->buffer + m->length) = input;
                m->length++;
            } else {
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (m->length > 0) {
                char *start = m->buffer;
                char *end = m->buffer + m->length - 1;
                
                while (start < end) {
                    char temp = *start;
                    *start = *end;
                    *end = temp;
                    start++;
                    end--;
                }
                m->current = STATE_OUTPUT;
            } else {
                m->current = STATE_START;
            }
            break;
            
        case STATE_OUTPUT:
            if (m->length > 0) {
                for (size_t i = 0; i < m->length; i++) {
                    putchar(*(m->buffer + i));
                }
                putchar('\n');
                m->processed_count++;
                m->current = STATE_END;
            } else {
                m->current = STATE_START;
            }
            break;
            
        case STATE_END:
            if (input == 'R') {
                m->current = STATE_START;
                memset(m->buffer, 0, MAX_INPUT_LEN);
                m->length = 0;
            }
            break;
            
        default:
            m->current = STATE_START;
            break;
    }
    
    return 1;
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    printf("State Machine Controller\n");
    printf("Commands: B=Begin, E=End input, R=Reset, Q=Quit\n");
    printf("Enter commands followed by data:\n");
    
    int running = 1;
    while (running) {
        int c = getchar();
        if (c == EOF || c == 'Q') {
            running = 0;
            continue;
        }
        
        if (c == '\n') continue;
        
        if (!machine_transition(&m, (char)c)) {
            printf("Error: Invalid transition\n");
            machine_init(&m);
        }
        
        if (m.current == STATE_END) {
            printf("Processed %d strings. Press R to reset.\n", m.processed_count);
        }
    }
    
    printf("Final count: %d strings processed\n", m.processed_count);
    return 0;
}