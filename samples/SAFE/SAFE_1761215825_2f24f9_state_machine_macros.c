//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 256

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
    int processed_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->processed_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int handle_idle(state_machine_t *machine) {
    printf("Enter text to process (or 'quit' to exit): ");
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (strcmp(input, "quit") == 0) {
        return 0;
    }
    
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        printf("Input too long\n");
        return -1;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return -1;
    }
    
    if (machine->buffer_len + input_len >= MAX_BUFFER) {
        printf("Buffer full\n");
        return -1;
    }
    
    memcpy(machine->buffer + machine->buffer_len, input, input_len);
    machine->buffer_len += input_len;
    TRANSITION(machine, STATE_READING);
    return 1;
}

int handle_reading(state_machine_t *machine) {
    printf("Read %zu characters into buffer\n", machine->buffer_len);
    TRANSITION(machine, STATE_PROCESSING);
    return 1;
}

int handle_processing(state_machine_t *machine) {
    int digit_count = 0;
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (IS_DIGIT(machine->buffer[i])) {
            digit_count++;
        }
    }
    
    machine->processed_count = digit_count;
    printf("Found %d digits in buffer\n", digit_count);
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

int handle_writing(state_machine_t *machine) {
    printf("Processed data: Buffer contains %zu characters, %d digits found\n", 
           machine->buffer_len, machine->processed_count);
    
    machine->buffer_len = 0;
    machine->processed_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *machine) {
    printf("Error state reached. Resetting machine.\n");
    initialize_machine(machine);
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int run_state_machine(state_machine_t *machine) {
    int result = 1;
    
    switch (machine->state) {
        case STATE_IDLE:
            result = handle_idle(machine);
            break;
        case STATE_READING:
            result = handle_reading(machine);
            break;
        case STATE_PROCESSING:
            result = handle_processing(machine);
            break;
        case STATE_WRITING:
            result = handle_writing(machine);
            break;
        case STATE_ERROR:
            result = handle_error(machine);
            break;
        default:
            printf("Invalid state: %d\n", machine->state);
            TRANSITION(machine, STATE_ERROR);
            result = -1;
            break;
    }
    
    if (result < 0) {
        TRANSITION(machine, STATE_ERROR);
    }
    
    return result;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Digit Counter\n");
    printf("Type 'quit' to exit\n\n");
    
    int running = 1;
    while (running) {
        int result = run_state_machine(&machine);
        
        if (result == 0) {
            running = 0;
        } else if (result < 0) {
            printf("Fatal error occurred\n");
            running = 0;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}