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
#define MAX_BUFFER 50

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buf_len;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->buf_len = 0;
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

static void handle_idle(state_machine_t *machine, const char *input, size_t len) {
    if (machine->state != STATE_IDLE) return;
    
    if (!validate_input(input, len)) {
        machine->error_code = 1;
        TRANSITION(machine, STATE_ERROR);
        return;
    }
    
    if (len > 0) {
        TRANSITION(machine, STATE_READING);
    }
}

static void handle_reading(state_machine_t *machine, const char *input, size_t len) {
    if (machine->state != STATE_READING) return;
    
    if (!validate_input(input, len)) {
        machine->error_code = 2;
        TRANSITION(machine, STATE_ERROR);
        return;
    }
    
    size_t copy_len = len;
    if (copy_len > MAX_BUFFER - 1) {
        copy_len = MAX_BUFFER - 1;
    }
    
    memcpy(machine->buffer, input, copy_len);
    machine->buffer[copy_len] = '\0';
    machine->buf_len = copy_len;
    
    TRANSITION(machine, STATE_PROCESSING);
}

static void handle_processing(state_machine_t *machine) {
    if (machine->state != STATE_PROCESSING) return;
    
    if (machine->buf_len == 0) {
        machine->error_code = 3;
        TRANSITION(machine, STATE_ERROR);
        return;
    }
    
    for (size_t i = 0; i < machine->buf_len; i++) {
        if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
            machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(machine, STATE_WRITING);
}

static void handle_writing(state_machine_t *machine) {
    if (machine->state != STATE_WRITING) return;
    
    if (printf("Processed: %s\n", machine->buffer) < 0) {
        machine->error_code = 4;
        TRANSITION(machine, STATE_ERROR);
        return;
    }
    
    TRANSITION(machine, STATE_IDLE);
}

static void handle_error(state_machine_t *machine) {
    if (machine->state != STATE_ERROR) return;
    
    if (printf("Error %d occurred\n", machine->error_code) < 0) {
        exit(1);
    }
    
    initialize_machine(machine);
}

static void process_input(state_machine_t *machine, const char *input, size_t len) {
    switch (machine->state) {
        case STATE_IDLE:
            handle_idle(machine, input, len);
            break;
        case STATE_READING:
            handle_reading(machine, input, len);
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
            machine->error_code = 5;
            TRANSITION(machine, STATE_ERROR);
            break;
    }
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("Enter text (empty line to exit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        process_input(&machine, input, len);
        
        while (machine.state != STATE_IDLE && machine.state != STATE_ERROR) {
            process_input(&machine, NULL, 0);
        }
        
        if (machine.state == STATE_ERROR) {
            process_input(&machine, NULL, 0);
        }
    }
    
    return 0;
}