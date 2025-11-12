//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
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
    char buffer[MAX_INPUT];
    size_t position;
    int result;
} Machine;

void machine_init(Machine *m) {
    m->current = STATE_START;
    m->position = 0;
    m->result = 0;
    memset(m->buffer, 0, MAX_INPUT);
}

State handle_start(Machine *m) {
    if (m->position >= MAX_INPUT - 1) {
        return STATE_END;
    }
    return STATE_READING;
}

State handle_reading(Machine *m, const char *input) {
    if (input == NULL) {
        return STATE_END;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT) {
        return STATE_END;
    }
    
    if (m->position + input_len >= MAX_INPUT - 1) {
        return STATE_END;
    }
    
    char *dest = m->buffer + m->position;
    const char *src = input;
    while (*src && m->position < MAX_INPUT - 1) {
        *dest = *src;
        dest++;
        src++;
        m->position++;
    }
    *dest = '\0';
    
    return STATE_PROCESSING;
}

State handle_processing(Machine *m) {
    if (m->position == 0) {
        return STATE_END;
    }
    
    char *ptr = m->buffer;
    int sum = 0;
    
    while (*ptr && ptr < m->buffer + m->position) {
        if (isdigit((unsigned char)*ptr)) {
            sum += (*ptr - '0');
        }
        ptr++;
    }
    
    m->result = sum;
    return STATE_OUTPUT;
}

State handle_output(Machine *m) {
    printf("Sum of digits: %d\n", m->result);
    return STATE_END;
}

int main(void) {
    Machine machine;
    machine_init(&machine);
    
    char input[MAX_INPUT];
    
    while (machine.current != STATE_END) {
        switch (machine.current) {
            case STATE_START:
                machine.current = handle_start(&machine);
                break;
                
            case STATE_READING:
                printf("Enter text (empty to finish): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    machine.current = STATE_END;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                
                if (strlen(input) == 0) {
                    machine.current = STATE_PROCESSING;
                } else {
                    machine.current = handle_reading(&machine, input);
                }
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
    
    return 0;
}