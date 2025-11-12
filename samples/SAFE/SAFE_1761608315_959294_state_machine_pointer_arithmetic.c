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
} Machine;

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

State handle_start(Machine *m, const char *input) {
    if (input == NULL || *input == '\0') {
        return STATE_END;
    }
    m->length = 0;
    m->processed_count = 0;
    return STATE_READING;
}

State handle_reading(Machine *m, const char *input) {
    const char *ptr = input;
    while (*ptr != '\0' && m->length < MAX_INPUT_LEN - 1) {
        if (is_valid_char(*ptr)) {
            *(m->buffer + m->length) = *ptr;
            m->length++;
        }
        ptr++;
    }
    *(m->buffer + m->length) = '\0';
    return STATE_PROCESSING;
}

State handle_processing(Machine *m) {
    if (m->length == 0) {
        return STATE_END;
    }
    
    char *start = m->buffer;
    char *end = m->buffer + m->length;
    
    while (start < end) {
        if (*start >= 'a' && *start <= 'z') {
            *start = *start - 'a' + 'A';
        }
        start++;
    }
    
    m->processed_count++;
    return STATE_OUTPUT;
}

State handle_output(Machine *m) {
    printf("Processed string: %s\n", m->buffer);
    printf("Total processed: %d\n", m->processed_count);
    return STATE_END;
}

void run_state_machine(const char *input) {
    Machine machine;
    machine.current = STATE_START;
    
    while (machine.current != STATE_END) {
        switch (machine.current) {
            case STATE_START:
                machine.current = handle_start(&machine, input);
                break;
            case STATE_READING:
                machine.current = handle_reading(&machine, input);
                break;
            case STATE_PROCESSING:
                machine.current = handle_processing(&machine);
                break;
            case STATE_OUTPUT:
                machine.current = handle_output(&machine);
                break;
            case STATE_END:
                break;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to process (letters and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    run_state_machine(input);
    
    return EXIT_SUCCESS;
}