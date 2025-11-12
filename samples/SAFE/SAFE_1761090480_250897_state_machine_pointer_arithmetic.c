//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_SIZE 128

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    INPUT_CHAR,
    INPUT_SPACE,
    INPUT_NEWLINE,
    INPUT_EOF
} InputType;

typedef struct {
    State current;
    char buffer[INPUT_SIZE];
    size_t pos;
    size_t word_count;
} Machine;

static InputType classify_input(int c) {
    if (c == EOF) return INPUT_EOF;
    if (c == ' ') return INPUT_SPACE;
    if (c == '\n') return INPUT_NEWLINE;
    if (c >= 0 && c <= 127) return INPUT_CHAR;
    return INPUT_EOF;
}

static State transition(State current, InputType input, Machine *machine) {
    char *buf = machine->buffer;
    size_t *pos = &machine->pos;
    
    switch (current) {
        case STATE_START:
            if (input == INPUT_CHAR) {
                if (*pos < INPUT_SIZE - 1) {
                    *(buf + *pos) = (char)input;
                    *pos += 1;
                }
                return STATE_READING;
            }
            return STATE_START;
            
        case STATE_READING:
            if (input == INPUT_CHAR) {
                if (*pos < INPUT_SIZE - 1) {
                    *(buf + *pos) = (char)input;
                    *pos += 1;
                }
                return STATE_READING;
            } else if (input == INPUT_SPACE || input == INPUT_NEWLINE) {
                if (*pos > 0) {
                    *(buf + *pos) = '\0';
                    return STATE_PROCESSING;
                }
                return STATE_READING;
            } else if (input == INPUT_EOF) {
                return STATE_END;
            }
            return STATE_READING;
            
        case STATE_PROCESSING:
            machine->word_count++;
            *pos = 0;
            return STATE_OUTPUT;
            
        case STATE_OUTPUT:
            if (input == INPUT_CHAR) {
                if (*pos < INPUT_SIZE - 1) {
                    *(buf + *pos) = (char)input;
                    *pos += 1;
                }
                return STATE_READING;
            } else if (input == INPUT_EOF) {
                return STATE_END;
            }
            return STATE_OUTPUT;
            
        case STATE_END:
            return STATE_END;
            
        default:
            return STATE_START;
    }
}

static void process_output(const Machine *machine) {
    const char *buf = machine->buffer;
    if (*buf != '\0') {
        printf("Word: %s\n", buf);
    }
}

static void run_state_machine(Machine *machine) {
    int c;
    InputType input;
    
    while (machine->current != STATE_END) {
        c = getchar();
        input = classify_input(c);
        
        State next = transition(machine->current, input, machine);
        
        if (next == STATE_PROCESSING && machine->current == STATE_READING) {
            process_output(machine);
        }
        
        machine->current = next;
    }
    
    if (machine->pos > 0 && machine->current == STATE_END) {
        process_output(machine);
        machine->word_count++;
    }
}

int main(void) {
    Machine machine;
    machine.current = STATE_START;
    machine.pos = 0;
    machine.word_count = 0;
    *(machine.buffer) = '\0';
    
    printf("Enter text (Ctrl+D to end):\n");
    run_state_machine(&machine);
    
    printf("\nTotal words: %zu\n", machine.word_count);
    
    return 0;
}