//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    STATE_FINAL,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char *input;
    size_t position;
    size_t length;
    int result;
} StateMachine;

void state_machine_init(StateMachine *sm, char *input) {
    if (sm == NULL || input == NULL) return;
    sm->current = STATE_START;
    sm->input = input;
    sm->position = 0;
    sm->length = strlen(input);
    sm->result = 0;
}

State state_start(StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    if (sm->position >= sm->length) return STATE_ERROR;
    if (*(sm->input + sm->position) == 'S') {
        sm->position++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

State state_reading(StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    if (sm->position >= sm->length) return STATE_ERROR;
    char current = *(sm->input + sm->position);
    if (current >= '0' && current <= '9') {
        sm->result = sm->result * 10 + (current - '0');
        sm->position++;
        return STATE_READING;
    } else if (current == 'E') {
        sm->position++;
        return STATE_PROCESSING;
    }
    return STATE_ERROR;
}

State state_processing(StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    if (sm->result < 0 || sm->result > 1000) return STATE_ERROR;
    sm->result *= 2;
    if (sm->result > 1000) sm->result = 1000;
    return STATE_FINAL;
}

State state_final(StateMachine *sm) {
    return STATE_FINAL;
}

State state_error(StateMachine *sm) {
    return STATE_ERROR;
}

typedef State (*StateHandler)(StateMachine*);

StateHandler handlers[STATE_COUNT] = {
    state_start,
    state_reading,
    state_processing,
    state_final,
    state_error
};

int run_state_machine(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    while (sm->current != STATE_FINAL && sm->current != STATE_ERROR) {
        if (sm->current < 0 || sm->current >= STATE_COUNT) {
            sm->current = STATE_ERROR;
            break;
        }
        StateHandler handler = *(handlers + sm->current);
        if (handler == NULL) {
            sm->current = STATE_ERROR;
            break;
        }
        sm->current = handler(sm);
    }
    
    return (sm->current == STATE_FINAL) ? sm->result : -1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter input (format S<digits>E): ");
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && *(input_buffer + len - 1) == '\n') {
        *(input_buffer + len - 1) = '\0';
        len--;
    }
    
    if (len == 0 || len >= MAX_INPUT_LEN) {
        printf("Invalid input length\n");
        return 1;
    }
    
    StateMachine sm;
    state_machine_init(&sm, input_buffer);
    
    int result = run_state_machine(&sm);
    
    if (result >= 0) {
        printf("Result: %d\n", result);
    } else {
        printf("Error processing input\n");
    }
    
    return 0;
}