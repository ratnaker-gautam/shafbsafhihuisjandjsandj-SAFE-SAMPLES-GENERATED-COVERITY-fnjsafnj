//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    size_t length;
} StateMachine;

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\n';
}

State handle_start(StateMachine* sm, char input) {
    if (is_valid_char(input)) {
        *(sm->buffer + sm->position) = input;
        sm->position++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine* sm, char input) {
    if (sm->position >= sm->length - 1) {
        return STATE_PROCESSING;
    }
    if (input == '\n') {
        return STATE_PROCESSING;
    }
    if (is_valid_char(input)) {
        *(sm->buffer + sm->position) = input;
        sm->position++;
    }
    return STATE_READING;
}

State handle_processing(StateMachine* sm) {
    if (sm->position == 0) {
        return STATE_DONE;
    }
    char* end = sm->buffer + sm->position - 1;
    char* start = sm->buffer;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return STATE_FINALIZING;
}

State handle_finalizing(StateMachine* sm) {
    *(sm->buffer + sm->position) = '\0';
    return STATE_DONE;
}

StateMachine* create_state_machine(size_t max_len) {
    if (max_len == 0 || max_len > MAX_INPUT_LEN) {
        return NULL;
    }
    
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) {
        return NULL;
    }
    
    sm->buffer = malloc(max_len);
    if (!sm->buffer) {
        free(sm);
        return NULL;
    }
    
    sm->current = STATE_START;
    sm->position = 0;
    sm->length = max_len;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        if (sm->buffer) {
            free(sm->buffer);
        }
        free(sm);
    }
}

int main(void) {
    StateMachine* sm = create_state_machine(MAX_INPUT_LEN);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    int c;
    while ((c = getchar()) != EOF && sm->current != STATE_DONE) {
        if (c == '\0') {
            continue;
        }
        
        char input = (char)c;
        
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, input);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, input);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_FINALIZING:
                sm->current = handle_finalizing(sm);
                break;
            case STATE_DONE:
                break;
        }
    }
    
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_FINALIZING) {
        sm->current = handle_finalizing(sm);
    }
    
    if (sm->current == STATE_DONE && sm->position > 0) {
        printf("Reversed: %s\n", sm->buffer);
    } else {
        printf("No valid input received\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}