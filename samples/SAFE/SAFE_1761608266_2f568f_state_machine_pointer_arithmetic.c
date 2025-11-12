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
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current = STATE_START;
    sm->length = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\n';
}

void process_buffer(StateMachine *sm) {
    if (sm == NULL || sm->length == 0) return;
    
    char *ptr = sm->buffer;
    char *end = ptr + sm->length;
    
    while (ptr < end) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 'a' + 'A';
        }
        ptr++;
    }
    
    sm->processed_count++;
}

int state_machine_step(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    
    switch (sm->current) {
        case STATE_START:
            if (is_valid_char(input)) {
                sm->current = STATE_READING;
                sm->buffer[0] = input;
                sm->length = 1;
            }
            break;
            
        case STATE_READING:
            if (input == '\n') {
                sm->current = STATE_PROCESSING;
            } else if (is_valid_char(input) && sm->length < MAX_INPUT_LEN - 1) {
                *(sm->buffer + sm->length) = input;
                sm->length++;
            } else {
                sm->current = STATE_START;
                sm->length = 0;
            }
            break;
            
        case STATE_PROCESSING:
            process_buffer(sm);
            sm->current = STATE_OUTPUT;
            break;
            
        case STATE_OUTPUT:
            if (sm->length > 0) {
                char *ptr = sm->buffer;
                char *end = ptr + sm->length;
                while (ptr < end) {
                    putchar(*ptr);
                    ptr++;
                }
                putchar('\n');
            }
            printf("Processed: %d\n", sm->processed_count);
            sm->current = STATE_END;
            break;
            
        case STATE_END:
            return 0;
    }
    
    return 1;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter text (max %d chars, letters and spaces only):\n", MAX_INPUT_LEN - 1);
    
    int running = 1;
    while (running) {
        int c = getchar();
        if (c == EOF) break;
        
        running = state_machine_step(&sm, (char)c);
        
        if (sm.current == STATE_END) {
            printf("Continue? (y/n): ");
            int response = getchar();
            if (response == 'y' || response == 'Y') {
                state_machine_init(&sm);
                printf("Enter text (max %d chars, letters and spaces only):\n", MAX_INPUT_LEN - 1);
                getchar();
            } else {
                break;
            }
        }
    }
    
    return 0;
}