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
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef struct {
    State current;
    char* buffer;
    size_t position;
    size_t capacity;
} StateMachine;

int state_machine_init(StateMachine* sm, size_t capacity) {
    if (!sm || capacity == 0 || capacity > MAX_INPUT_LEN) return 0;
    
    sm->buffer = malloc(capacity);
    if (!sm->buffer) return 0;
    
    sm->current = STATE_START;
    sm->position = 0;
    sm->capacity = capacity;
    
    *(sm->buffer) = '\0';
    return 1;
}

void state_machine_cleanup(StateMachine* sm) {
    if (sm && sm->buffer) {
        free(sm->buffer);
        sm->buffer = NULL;
    }
}

int state_machine_advance(StateMachine* sm, char input) {
    if (!sm || !sm->buffer) return 0;
    
    switch (sm->current) {
        case STATE_START:
            if (input == 'B' || input == 'b') {
                sm->current = STATE_READING;
                sm->position = 0;
                *(sm->buffer) = '\0';
            }
            break;
            
        case STATE_READING:
            if (input == 'E' || input == 'e') {
                sm->current = STATE_PROCESSING;
            } else if (sm->position < sm->capacity - 1) {
                *(sm->buffer + sm->position) = input;
                sm->position++;
                *(sm->buffer + sm->position) = '\0';
            }
            break;
            
        case STATE_PROCESSING:
            if (input == 'F' || input == 'f') {
                sm->current = STATE_FINALIZING;
            } else {
                sm->current = STATE_READING;
            }
            break;
            
        case STATE_FINALIZING:
            if (input == 'X' || input == 'x') {
                sm->current = STATE_DONE;
            } else {
                sm->current = STATE_PROCESSING;
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return 1;
}

void state_machine_process(StateMachine* sm) {
    if (!sm || !sm->buffer) return;
    
    switch (sm->current) {
        case STATE_PROCESSING:
            for (char* ptr = sm->buffer; ptr < sm->buffer + sm->position; ptr++) {
                if (*ptr >= 'a' && *ptr <= 'z') {
                    *ptr = *ptr - 'a' + 'A';
                }
            }
            break;
            
        case STATE_FINALIZING:
            if (sm->position > 0) {
                char* end = sm->buffer + sm->position - 1;
                while (end >= sm->buffer && *end == ' ') {
                    *end = '\0';
                    end--;
                    sm->position--;
                }
            }
            break;
            
        case STATE_DONE:
            printf("Final result: %s\n", sm->buffer);
            break;
            
        default:
            break;
    }
}

int main() {
    StateMachine sm;
    if (!state_machine_init(&sm, 128)) {
        fprintf(stderr, "Failed to initialize state machine\n");
        return 1;
    }
    
    char input[MAX_INPUT_LEN];
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Failed to read input\n");
        state_machine_cleanup(&sm);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!state_machine_advance(&sm, input[i])) {
            fprintf(stderr, "Error advancing state machine\n");
            state_machine_cleanup(&sm);
            return 1;
        }
        state_machine_process(&sm);
    }
    
    if (sm.current != STATE_DONE) {
        printf("Sequence incomplete. Current state: %d\n", sm.current);
        printf("Buffer content: %s\n", sm.buffer);
    }
    
    state_machine_cleanup(&sm);
    return 0;
}