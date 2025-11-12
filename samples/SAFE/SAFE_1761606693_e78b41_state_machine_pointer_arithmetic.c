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
    sm->current = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->length = 0;
    sm->processed_count = 0;
}

int is_valid_input(const char *input, size_t len) {
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    const char *ptr = input;
    while (ptr < input + len) {
        if (*ptr < 32 || *ptr > 126) return 0;
        ptr++;
    }
    return 1;
}

void process_buffer(StateMachine *sm) {
    if (sm->length == 0) return;
    
    char *start = sm->buffer;
    char *end = sm->buffer + sm->length;
    
    while (start < end) {
        if (*start >= 'a' && *start <= 'z') {
            *start = *start - 32;
        }
        start++;
    }
    sm->processed_count++;
}

State handle_start(StateMachine *sm) {
    printf("State machine started. Enter text (empty line to finish):\n");
    return STATE_READING;
}

State handle_reading(StateMachine *sm) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_END;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        printf("Invalid input. Please enter printable ASCII characters only.\n");
        return STATE_READING;
    }
    
    if (len == 0) {
        return STATE_PROCESSING;
    }
    
    if (sm->length + len < MAX_INPUT_LEN) {
        memcpy(sm->buffer + sm->length, input, len);
        sm->length += len;
        printf("Input stored. Current buffer size: %zu\n", sm->length);
    } else {
        printf("Buffer full. Moving to processing state.\n");
        return STATE_PROCESSING;
    }
    
    return STATE_READING;
}

State handle_processing(StateMachine *sm) {
    printf("Processing buffer...\n");
    process_buffer(sm);
    return STATE_OUTPUT;
}

State handle_output(StateMachine *sm) {
    printf("Processed output: ");
    
    char *ptr = sm->buffer;
    char *end = sm->buffer + sm->length;
    
    while (ptr < end) {
        putchar(*ptr);
        ptr++;
    }
    printf("\n");
    
    printf("Total strings processed: %d\n", sm->processed_count);
    
    state_machine_init(sm);
    
    printf("Continue? (y/n): ");
    char response[16];
    if (fgets(response, 16, stdin) != NULL) {
        if (response[0] == 'y' || response[0] == 'Y') {
            return STATE_START;
        }
    }
    
    return STATE_END;
}

State handle_end(StateMachine *sm) {
    printf("State machine terminated.\n");
    return STATE_END;
}

typedef State (*StateHandler)(StateMachine*);

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    StateHandler handlers[STATE_COUNT] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_output,
        handle_end
    };
    
    while (sm.current != STATE_END) {
        if (sm.current >= 0 && sm.current < STATE_COUNT) {
            sm.current = handlers[sm.current](&sm);
        } else {
            sm.current = STATE_END;
        }
    }
    
    return 0;
}