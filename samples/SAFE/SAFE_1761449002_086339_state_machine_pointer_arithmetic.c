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
    char buffer[MAX_INPUT_LEN];
    size_t position;
    int result;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->current = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->position = 0;
    sm->result = 0;
}

int is_valid_input(char c) {
    return (c >= '0' && c <= '9') || c == ' ' || c == '\n' || c == '\0';
}

State handle_start(StateMachine *sm, char input) {
    if (input == '\0') {
        return STATE_DONE;
    }
    if (input >= '0' && input <= '9') {
        *(sm->buffer + sm->position) = input;
        sm->position++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (input >= '0' && input <= '9') {
        if (sm->position >= MAX_INPUT_LEN - 1) {
            return STATE_DONE;
        }
        *(sm->buffer + sm->position) = input;
        sm->position++;
        return STATE_READING;
    }
    if (input == ' ' || input == '\n' || input == '\0') {
        *(sm->buffer + sm->position) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_DONE;
}

State handle_processing(StateMachine *sm) {
    char *endptr = NULL;
    long num = strtol(sm->buffer, &endptr, 10);
    if (endptr == sm->buffer || *endptr != '\0') {
        return STATE_DONE;
    }
    if (num < 0 || num > 1000000) {
        return STATE_DONE;
    }
    sm->result += (int)num;
    sm->position = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    return STATE_FINALIZING;
}

State handle_finalizing(StateMachine *sm, char input) {
    if (input == '\0' || input == '\n') {
        return STATE_DONE;
    }
    if (input == ' ') {
        return STATE_START;
    }
    return STATE_DONE;
}

void process_state_machine(StateMachine *sm, const char *input) {
    const char *ptr = input;
    while (sm->current != STATE_DONE && *ptr != '\0') {
        if (!is_valid_input(*ptr)) {
            sm->current = STATE_DONE;
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
            case STATE_FINALIZING:
                sm->current = handle_finalizing(sm, *ptr);
                break;
            case STATE_DONE:
                return;
        }
        
        ptr++;
    }
    
    if (sm->current == STATE_PROCESSING) {
        sm->current = handle_processing(sm);
    }
    if (sm->current == STATE_FINALIZING) {
        sm->current = STATE_DONE;
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter numbers separated by spaces: ");
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine_init(&sm);
    process_state_machine(&sm, input);
    
    printf("Sum: %d\n", sm.result);
    
    return 0;
}