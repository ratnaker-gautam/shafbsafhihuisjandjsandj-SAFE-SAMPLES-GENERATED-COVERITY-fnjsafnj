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
                m->current = STATE_START;
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
                
                m->processed_count++;
                m->current = STATE_OUTPUT;
            } else {
                m->current = STATE_START;
            }
            break;
            
        case STATE_OUTPUT:
            if (input == 'C') {
                if (m->length > 0) {
                    char *ptr = m->buffer;
                    while (ptr < m->buffer + m->length) {
                        putchar(*ptr);
                        ptr++;
                    }
                    putchar('\n');
                }
                m->current = STATE_END;
            }
            break;
            
        case STATE_END:
            if (input == 'R') {
                machine_init(m);
            }
            break;
    }
    
    return 1;
}

int main(void) {
    Machine m;
    machine_init(&m);
    
    printf("State Machine Controller\n");
    printf("Commands: B=Begin, E=End input, C=Continue, R=Reset\n");
    printf("Enter commands and data:\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') continue;
        
        if (!machine_transition(&m, (char)c)) {
            fprintf(stderr, "Machine error\n");
            return 1;
        }
        
        if (m.current == STATE_END) {
            printf("Processed strings: %d\n", m.processed_count);
            printf("Enter 'R' to reset or Ctrl+D to exit\n");
        }
    }
    
    return 0;
}