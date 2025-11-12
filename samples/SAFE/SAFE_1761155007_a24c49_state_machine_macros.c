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
#define MAX_BUFFER_SIZE 50

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define CHECK_BOUNDS(len) ((len) >= 0 && (len) < MAX_BUFFER_SIZE)

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->state = STATE_IDLE;
    machine->buffer_len = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

static void handle_idle(state_machine_t *machine, const char *input, size_t len) {
    if (machine == NULL) return;
    if (machine->state != STATE_IDLE) return;
    
    if (validate_input(input, len) && CHECK_BOUNDS(len)) {
        memcpy(machine->buffer, input, len);
        machine->buffer_len = len;
        machine->buffer[len] = '\0';
        TRANSITION(machine, STATE_READING);
    } else {
        TRANSITION(machine, STATE_ERROR);
    }
}

static void handle_reading(state_machine_t *machine) {
    if (machine == NULL) return;
    if (machine->state != STATE_READING) return;
    
    if (machine->buffer_len > 0) {
        for (size_t i = 0; i < machine->buffer_len; i++) {
            if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
                machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
            }
        }
        TRANSITION(machine, STATE_PROCESSING);
    } else {
        TRANSITION(machine, STATE_ERROR);
    }
}

static void handle_processing(state_machine_t *machine) {
    if (machine == NULL) return;
    if (machine->state != STATE_PROCESSING) return;
    
    if (machine->buffer_len > 0) {
        TRANSITION(machine, STATE_WRITING);
    } else {
        TRANSITION(machine, STATE_ERROR);
    }
}

static void handle_writing(state_machine_t *machine) {
    if (machine == NULL) return;
    if (machine->state != STATE_WRITING) return;
    
    if (machine->buffer_len > 0) {
        printf("Processed output: %s\n", machine->buffer);
        TRANSITION(machine, STATE_IDLE);
    } else {
        TRANSITION(machine, STATE_ERROR);
    }
}

static void handle_error(state_machine_t *machine) {
    if (machine == NULL) return;
    if (machine->state != STATE_ERROR) return;
    
    printf("Error state encountered\n");
    initialize_machine(machine);
}

static void process_state_machine(state_machine_t *machine, const char *input, size_t len) {
    if (machine == NULL) return;
    
    switch (machine->state) {
        case STATE_IDLE:
            handle_idle(machine, input, len);
            break;
        case STATE_READING:
            handle_reading(machine);
            break;
        case STATE_PROCESSING:
            handle_processing(machine);
            break;
        case STATE_WRITING:
            handle_writing(machine);
            break;
        case STATE_ERROR:
            handle_error(machine);
            break;
        default:
            TRANSITION(machine, STATE_ERROR);
            break;
    }
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("Enter text to process (empty line to exit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (input[0] == '\n') {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        process_state_machine(&machine, input, len);
        
        while (machine.state != STATE_IDLE && machine.state != STATE_ERROR) {
            process_state_machine(&machine, NULL, 0);
        }
    }
    
    return 0;
}