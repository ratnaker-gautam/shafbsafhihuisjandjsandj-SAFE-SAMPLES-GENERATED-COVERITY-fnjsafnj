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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->error_code = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static int handle_idle(state_machine_t *machine, const char *input, size_t len) {
    if (!validate_input(input, len)) {
        machine->error_code = 1;
        TRANSITION(machine, STATE_ERROR);
        return 0;
    }
    
    if (len > 0) {
        machine->buffer_len = len < MAX_BUFFER_SIZE ? len : MAX_BUFFER_SIZE - 1;
        memcpy(machine->buffer, input, machine->buffer_len);
        machine->buffer[machine->buffer_len] = '\0';
        TRANSITION(machine, STATE_READING);
        return 1;
    }
    return 0;
}

static int handle_reading(state_machine_t *machine) {
    if (machine->buffer_len == 0) {
        TRANSITION(machine, STATE_IDLE);
        return 0;
    }
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
            machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
        }
    }
    TRANSITION(machine, STATE_PROCESSING);
    return 1;
}

static int handle_processing(state_machine_t *machine) {
    size_t new_len = 0;
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if ((machine->buffer[i] >= 'A' && machine->buffer[i] <= 'Z') || 
            (machine->buffer[i] >= '0' && machine->buffer[i] <= '9')) {
            machine->buffer[new_len++] = machine->buffer[i];
        }
    }
    machine->buffer_len = new_len;
    machine->buffer[machine->buffer_len] = '\0';
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

static int handle_writing(state_machine_t *machine) {
    if (machine->buffer_len > 0) {
        printf("Processed: %s\n", machine->buffer);
    } else {
        printf("No valid characters found\n");
    }
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

static int handle_error(state_machine_t *machine) {
    printf("Error: Invalid input (code %d)\n", machine->error_code);
    machine->error_code = 0;
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int process_state(state_machine_t *machine, const char *input, size_t len) {
    switch (machine->state) {
        case STATE_IDLE:
            return handle_idle(machine, input, len);
        case STATE_READING:
            return handle_reading(machine);
        case STATE_PROCESSING:
            return handle_processing(machine);
        case STATE_WRITING:
            return handle_writing(machine);
        case STATE_ERROR:
            return handle_error(machine);
        default:
            machine->error_code = 2;
            TRANSITION(machine, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Enter text (empty line to exit):\n");
    
    while (1) {
        if (machine.state == STATE_IDLE) {
            if (fgets(input, sizeof(input), stdin) == NULL) break;
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }
            
            if (len == 0) break;
        }
        
        if (!process_state(&machine, input, strlen(input))) {
            if (machine.state == STATE_ERROR) {
                process_state(&machine, NULL, 0);
            }
        }
        
        if (machine.state != STATE_IDLE && machine.state != STATE_ERROR) {
            process_state(&machine, NULL, 0);