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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) < (max))

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

int handle_idle(state_machine_t *machine, char input) {
    if (input == 'S') {
        TRANSITION(machine, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(state_machine_t *machine, char input) {
    if (input == 'E') {
        if (machine->buffer_len > 0) {
            TRANSITION(machine, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
    } else if (input == 'Q') {
        TRANSITION(machine, STATE_IDLE);
        machine->buffer_len = 0;
        return 1;
    } else {
        if (CHECK_BOUNDS(machine->buffer_len, MAX_BUFFER - 1) && IS_VALID_CHAR(input)) {
            machine->buffer[machine->buffer_len++] = input;
            return 1;
        } else {
            TRANSITION(machine, STATE_ERROR);
            return 0;
        }
    }
}

int handle_processing(state_machine_t *machine) {
    if (machine->buffer_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
            machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
        }
    }
    
    machine->processed_count++;
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

int handle_writing(state_machine_t *machine) {
    if (machine->buffer_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    printf("Processed: ");
    for (size_t i = 0; i < machine->buffer_len; i++) {
        putchar(machine->buffer[i]);
    }
    printf("\n");
    
    machine->buffer_len = 0;
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *machine) {
    printf("Error state reached. Resetting.\n");
    machine->buffer_len = 0;
    machine->processed_count = 0;
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int process_input(state_machine_t *machine, char input) {
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input);
        case STATE_READING:
            return handle_reading(machine, input);
        case STATE_PROCESSING:
            return handle_processing(machine);
        case STATE_WRITING:
            return handle_writing(machine);
        case STATE_ERROR:
            return handle_error(machine);
        default:
            TRANSITION(machine, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Commands: S=Start, E=End, Q=Quit reading, Ctrl+D=Exit\n");
    printf("States: 0=Idle, 1=Reading, 2=Processing, 3=Writing, 4=Error\n");
    
    while (1) {
        printf("\nCurrent state: %d, Processed: %d\n", machine.state, machine.processed_count);
        printf("Enter command or text: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (len == 1) {
            if (!process_input(&machine, input[0])) {
                printf("Invalid transition\n");
            }
        } else {
            if (machine.state == STATE_READING) {
                for (size_t i = 0; i < len; i++) {
                    if (!process_input(&machine, input[i])) {
                        break;
                    }
                }
            } else {
                printf("Multiple characters only allowed in READING state\n");
            }
        }
    }
    
    return 0;