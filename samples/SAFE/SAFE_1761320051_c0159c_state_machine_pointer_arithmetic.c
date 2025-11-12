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
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current = STATE_START;
    sm->pos = 0;
    sm->result = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int is_valid_input(char c) {
    return (c >= '0' && c <= '9') || c == ' ' || c == '\n' || c == '\0';
}

State handle_start(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == '\0') return STATE_END;
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input >= '0' && input <= '9') {
        if (sm->pos >= MAX_INPUT_LEN - 1) return STATE_END;
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    if (input == ' ' || input == '\n' || input == '\0') {
        *(sm->buffer + sm->pos) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    char *endptr = NULL;
    long num = strtol(sm->buffer, &endptr, 10);
    if (endptr == sm->buffer || *endptr != '\0') return STATE_END;
    if (num < 0 || num > 10000) return STATE_END;
    sm->result += (int)num;
    sm->pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    printf("Current sum: %d\n", sm->result);
    return STATE_START;
}

void process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    const char *ptr = input;
    while (*ptr != '\0' && sm->current != STATE_END) {
        if (!is_valid_input(*ptr)) {
            sm->current = STATE_END;
            break;
        }
        
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, *ptr);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, *ptr);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                break;
            case STATE_END:
                return;
        }
        
        if (sm->current == STATE_PROCESSING || sm->current == STATE_OUTPUT) {
            continue;
        }
        ptr++;
    }
    
    if (sm->current == STATE_READING && sm->pos > 0) {
        sm->current = STATE_PROCESSING;
    }
    
    while (sm->current != STATE_END && sm->current != STATE_START) {
        switch (sm->current) {
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                break;
            default:
                sm->current = STATE_END;
                break;
        }
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    printf("Enter numbers separated by spaces (max %d chars): ", MAX_INPUT_LEN * 2 - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(&sm, input);
    
    if (sm.current == STATE_END) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    printf("Final sum: %d\n", sm.result);
    return 0;
}