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
    size_t length;
    size_t position;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->current = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->length = 0;
    sm->position = 0;
}

int is_valid_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ';
}

State handle_start(StateMachine *sm, char input) {
    if (is_valid_char(input)) {
        sm->buffer[sm->length] = input;
        sm->length++;
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(StateMachine *sm, char input) {
    if (sm->length >= MAX_INPUT_LEN - 1) {
        return STATE_PROCESSING;
    }
    if (input == '\n' || input == '\0') {
        return STATE_PROCESSING;
    }
    if (is_valid_char(input)) {
        sm->buffer[sm->length] = input;
        sm->length++;
        return STATE_READING;
    }
    return STATE_PROCESSING;
}

State handle_processing(StateMachine *sm) {
    if (sm->length == 0) {
        return STATE_DONE;
    }
    char *start = sm->buffer;
    char *end = start + sm->length - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return STATE_FINALIZING;
}

State handle_finalizing(StateMachine *sm) {
    if (sm->position < sm->length) {
        putchar(*(sm->buffer + sm->position));
        sm->position++;
        return STATE_FINALIZING;
    }
    putchar('\n');
    return STATE_DONE;
}

void process_input(StateMachine *sm, const char *input) {
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    for (size_t i = 0; i < input_len; i++) {
        switch (sm->current) {
            case STATE_START:
                sm->current = handle_start(sm, input[i]);
                break;
            case STATE_READING:
                sm->current = handle_reading(sm, input[i]);
                break;
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                i--;
                break;
            case STATE_FINALIZING:
                sm->current = handle_finalizing(sm);
                i--;
                break;
            case STATE_DONE:
                return;
        }
    }
    if (sm->current == STATE_READING) {
        sm->current = STATE_PROCESSING;
    }
    while (sm->current != STATE_DONE) {
        switch (sm->current) {
            case STATE_PROCESSING:
                sm->current = handle_processing(sm);
                break;
            case STATE_FINALIZING:
                sm->current = handle_finalizing(sm);
                break;
            default:
                break;
        }
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    printf("Enter text (letters and spaces only): ");
    
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(&sm, input);
    
    return EXIT_SUCCESS;
}