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
#define MAX_BUFFER_SIZE 200

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    if (!machine) return;
    machine->state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->error_code = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

static int handle_idle(state_machine_t *machine, const char *input) {
    if (!machine || !input) {
        if (machine) machine->error_code = 1;
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        machine->error_code = 2;
        return -1;
    }
    
    if (input_len > 0) {
        machine->buffer_len = 0;
        TRANSITION(machine, STATE_READING);
    }
    return 0;
}

static int handle_reading(state_machine_t *machine, const char *input) {
    if (!machine || !input) {
        if (machine) machine->error_code = 3;
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (machine->buffer_len + input_len >= MAX_BUFFER_SIZE) {
        machine->error_code = 4;
        return -1;
    }
    
    memcpy(machine->buffer + machine->buffer_len, input, input_len);
    machine->buffer_len += input_len;
    
    if (machine->buffer_len > 0) {
        TRANSITION(machine, STATE_PROCESSING);
    }
    return 0;
}

static int handle_processing(state_machine_t *machine) {
    if (!machine) return -1;
    
    if (machine->buffer_len == 0) {
        machine->error_code = 5;
        return -1;
    }
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
            machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(machine, STATE_WRITING);
    return 0;
}

static int handle_writing(state_machine_t *machine) {
    if (!machine) return -1;
    
    if (machine->buffer_len == 0) {
        machine->error_code = 6;
        return -1;
    }
    
    if (fwrite(machine->buffer, 1, machine->buffer_len, stdout) != machine->buffer_len) {
        machine->error_code = 7;
        return -1;
    }
    
    printf("\n");
    machine->buffer_len = 0;
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int handle_error(state_machine_t *machine) {
    if (!machine) return -1;
    fprintf(stderr, "Error code: %d\n", machine->error_code);
    initialize_machine(machine);
    return -1;
}

static int process_input(state_machine_t *machine, const char *input) {
    if (!machine || !input) {
        return -1;
    }
    
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input);
        case STATE_READING:
            return handle_reading(machine, input);
        case STATE_PROCESSING:
            if (handle_processing(machine) == 0) {
                return handle_writing(machine);
            }
            break;
        case STATE_WRITING:
            return handle_writing(machine);
        case STATE_ERROR:
            return handle_error(machine);
        default:
            machine->error_code = 8;
            TRANSITION(machine, STATE_ERROR);
            return -1;
    }
    return -1;
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN + 1];
    
    initialize_machine(&machine);
    
    printf("Enter text (empty line to exit):\n");
    
    while (fgets(input, sizeof(input), stdin)) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (process_input(&machine, input) < 0 && machine.state == STATE_ERROR) {
            handle_error(&machine);
        }
        
        printf("Current state: %d\n", machine.state);
    }
    
    if (machine.state != STATE_IDLE && machine.state != STATE_ERROR) {
        if (machine.state == STATE_PROCESSING) {
            handle_processing(&machine);
        }
        if (machine.state == STATE_WRITING