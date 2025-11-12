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
#define MAX_OUTPUT_LEN 200

#define TRANSITION(machine, new_state) do { (machine)->state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_OUTPUT_LEN];
    size_t input_len;
    size_t output_len;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
    memset(machine->output_buffer, 0, MAX_OUTPUT_LEN);
}

int handle_idle(state_machine_t *machine) {
    printf("Enter text (max %d chars, 'quit' to exit): ", MAX_INPUT_LEN - 1);
    
    char temp_buffer[MAX_INPUT_LEN];
    if (fgets(temp_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(temp_buffer);
    if (len > 0 && temp_buffer[len - 1] == '\n') {
        temp_buffer[len - 1] = '\0';
        len--;
    }
    
    if (strcmp(temp_buffer, "quit") == 0) {
        return 0;
    }
    
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(temp_buffer[i])) {
            TRANSITION(machine, STATE_ERROR);
            return -1;
        }
    }
    
    strncpy(machine->input_buffer, temp_buffer, MAX_INPUT_LEN - 1);
    machine->input_buffer[MAX_INPUT_LEN - 1] = '\0';
    machine->input_len = len;
    TRANSITION(machine, STATE_READING);
    return 1;
}

int handle_reading(state_machine_t *machine) {
    if (machine->input_len == 0) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    TRANSITION(machine, STATE_PROCESSING);
    return 1;
}

int handle_processing(state_machine_t *machine) {
    if (!CHECK_BOUNDS(machine->input_len, MAX_INPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    
    machine->output_len = 0;
    for (size_t i = 0; i < machine->input_len; i++) {
        if (!CHECK_BOUNDS(machine->output_len, MAX_OUTPUT_LEN - 1)) {
            TRANSITION(machine, STATE_ERROR);
            return -1;
        }
        
        char c = machine->input_buffer[i];
        if (c >= 'a' && c <= 'z') {
            machine->output_buffer[machine->output_len++] = c - 32;
        } else if (c >= 'A' && c <= 'Z') {
            machine->output_buffer[machine->output_len++] = c + 32;
        } else {
            machine->output_buffer[machine->output_len++] = c;
        }
    }
    
    if (!CHECK_BOUNDS(machine->output_len, MAX_OUTPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    
    machine->output_buffer[machine->output_len] = '\0';
    TRANSITION(machine, STATE_WRITING);
    return 1;
}

int handle_writing(state_machine_t *machine) {
    if (!CHECK_BOUNDS(machine->output_len, MAX_OUTPUT_LEN)) {
        TRANSITION(machine, STATE_ERROR);
        return -1;
    }
    
    printf("Processed: %s\n", machine->output_buffer);
    printf("Input length: %zu, Output length: %zu\n\n", 
           machine->input_len, machine->output_len);
    
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int handle_error(state_machine_t *machine) {
    printf("Error: Invalid input or buffer overflow\n");
    initialize_machine(machine);
    TRANSITION(machine, STATE_IDLE);
    return 1;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Text Case Converter State Machine\n");
    printf("=================================\n\n");
    
    int running = 1;
    while (running) {
        int result = 1;
        
        switch (machine.state) {
            case STATE_IDLE:
                result = handle_idle(&machine);
                if (result == 0) running = 0;
                break;
            case STATE_READING:
                result = handle_reading(&machine);
                break;
            case STATE_PROCESSING:
                result = handle_processing(&machine);
                break;