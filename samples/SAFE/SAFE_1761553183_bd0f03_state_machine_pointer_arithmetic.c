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

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '=';
}

State handle_start(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == '+') {
        sm->result = 0;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm == NULL || sm->pos >= MAX_INPUT_LEN - 1) return STATE_END;
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    } else if (input == '=') {
        *(sm->buffer + sm->pos) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    char *endptr;
    long value = strtol(sm->buffer, &endptr, 10);
    if (endptr == sm->buffer || *endptr != '\0') return STATE_END;
    if (value < -1000 || value > 1000) return STATE_END;
    sm->result += (int)value;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    printf("Current total: %d\n", sm->result);
    sm->pos = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_READING;
}

void process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    const char *ptr = input;
    while (*ptr != '\0' && sm->current != STATE_END) {
        if (!is_valid_char(*ptr)) {
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
                continue;
            case STATE_OUTPUT:
                sm->current = handle_output(sm);
                continue;
            case STATE_END:
                return;
        }
        ptr++;
    }
    
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_OUTPUT) {
        sm->current = handle_output(sm);
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter sequence (format: +NUM= +NUM= ...): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        return 1;
    }
    
    process_input(&sm, input);
    
    if (sm.current == STATE_END) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    printf("Final total: %d\n", sm.result);
    return 0;
}