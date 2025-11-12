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

void machine_init(Machine *m) {
    m->current = STATE_START;
    memset(m->buffer, 0, MAX_INPUT_LEN);
    m->length = 0;
    m->processed_count = 0;
}

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

State handle_start(Machine *m, char input) {
    if (input == 'B') {
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading(Machine *m, char input) {
    if (input == 'E') {
        return STATE_PROCESSING;
    }
    if (m->length < MAX_INPUT_LEN - 1 && is_valid_char(input)) {
        *(m->buffer + m->length) = input;
        m->length++;
        return STATE_READING;
    }
    return STATE_END;
}

State handle_processing(Machine *m) {
    if (m->length == 0) {
        return STATE_END;
    }
    
    char *start = m->buffer;
    char *end = m->buffer + m->length - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    m->processed_count++;
    return STATE_OUTPUT;
}

State handle_output(Machine *m) {
    if (m->length > 0) {
        char *ptr = m->buffer;
        while (ptr < m->buffer + m->length) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    machine_init(m);
    return STATE_START;
}

int main(void) {
    Machine machine;
    machine_init(&machine);
    
    printf("Enter characters (B to begin, E to end processing):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') {
            continue;
        }
        
        char c = (char)input_char;
        
        switch (machine.current) {
            case STATE_START:
                machine.current = handle_start(&machine, c);
                break;
            case STATE_READING:
                machine.current = handle_reading(&machine, c);
                break;
            case STATE_PROCESSING:
                machine.current = handle_processing(&machine);
                break;
            case STATE_OUTPUT:
                machine.current = handle_output(&machine);
                break;
            case STATE_END:
                printf("Processing complete. Total processed: %d\n", machine.processed_count);
                return 0;
        }
        
        if (machine.current == STATE_PROCESSING) {
            machine.current = handle_processing(&machine);
        }
        if (machine.current == STATE_OUTPUT) {
            machine.current = handle_output(&machine);
        }
    }
    
    if (machine.current != STATE_END) {
        printf("Unexpected end of input.\n");
    }
    
    return 0;
}