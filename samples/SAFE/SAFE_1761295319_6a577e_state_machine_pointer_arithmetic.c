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
    return (c >= '0' && c <= '9') || c == '+' || c == '-';
}

State handle_start(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input == '+') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    } else if (input == '-') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    } else if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->pos) = input;
        sm->pos++;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_END;
    if (input >= '0' && input <= '9') {
        if (sm->pos < MAX_INPUT_LEN - 1) {
            *(sm->buffer + sm->pos) = input;
            sm->pos++;
            return STATE_READING;
        }
    } else if (input == '\n' || input == '\0') {
        return STATE_PROCESSING;
    }
    return STATE_END;
}

State handle_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    if (sm->pos == 0) return STATE_END;
    
    char *endptr = NULL;
    long val = strtol(sm->buffer, &endptr, 10);
    if (endptr != sm->buffer + sm->pos) return STATE_END;
    if (val < -1000 || val > 1000) return STATE_END;
    
    sm->result = (int)val;
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    if (sm == NULL) return STATE_END;
    printf("Processed number: %d\n", sm->result);
    return STATE_END;
}

void process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    const char *ptr = input;
    while (*ptr != '\0' && sm->current != STATE_END) {
        char c = *ptr;
        ptr++;
        
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, c);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, c);
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
    }
    
    if (sm->current == STATE_READING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_OUTPUT) {
        sm->current = handle_output(sm);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    StateMachine sm;
    
    printf("Enter a signed integer (-1000 to 1000): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine_init(&sm);
    process_input(&sm, input);
    
    return EXIT_SUCCESS;
}