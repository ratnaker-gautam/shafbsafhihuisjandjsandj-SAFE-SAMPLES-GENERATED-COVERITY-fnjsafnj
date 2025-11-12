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

static void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    memset(machine->output_buffer, 0, sizeof(machine->output_buffer));
}

static int handle_idle(state_machine_t *machine) {
    printf("Enter text (max %d chars, 'q' to quit): ", MAX_INPUT_LEN - 1);
    
    char line[MAX_INPUT_LEN + 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    if (strcmp(line, "q") == 0) {
        return -1;
    }
    
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
        return -2;
    }
    
    memcpy(machine->input_buffer, line, len + 1);
    machine->input_len = len;
    TRANSITION(machine, STATE_READING);
    return 0;
}

static int handle_reading(state_machine_t *machine) {
    for (size_t i = 0; i < machine->input_len; i++) {
        if (!IS_VALID_CHAR(machine->input_buffer[i])) {
            TRANSITION(machine, STATE_ERROR);
            return -1;
        }
    }
    TRANSITION(machine, STATE_PROCESSING);
    return 0;
}

static int handle_processing(state_machine_t *machine) {
    machine->output_len = 0;
    
    for (size_t i = 0; i < machine->input_len; i++) {
        char c = machine->input_buffer[i];
        
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        } else if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        
        if (!CHECK_BOUNDS(machine->output_len, MAX_OUTPUT_LEN - 1)) {
            TRANSITION(machine, STATE_ERROR);
            return -1;
        }
        
        machine->output_buffer[machine->output_len++] = c;
    }
    
    machine->output_buffer[machine->output_len] = '\0';
    TRANSITION(machine, STATE_WRITING);
    return 0;
}

static int handle_writing(state_machine_t *machine) {
    printf("Processed: %s\n", machine->output_buffer);
    TRANSITION(machine, STATE_IDLE);
    return 0;
}

static int handle_error(state_machine_t *machine) {
    printf("Error: Invalid input or buffer overflow\n");
    initialize_machine(machine);
    return 0;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("State Machine Text Processor (case inverter)\n");
    
    while (1) {
        int result = 0;
        
        switch (machine.state) {
            case STATE_IDLE:
                result = handle_idle(&machine);
                if (result == -1) {
                    printf("Goodbye!\n");
                    return 0;
                } else if (result == -2) {
                    TRANSITION(&machine, STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                handle_reading(&machine);
                break;
                
            case STATE_PROCESSING:
                handle_processing(&machine);
                break;
                
            case STATE_WRITING:
                handle_writing(&machine);
                break;
                
            case STATE_ERROR:
                handle_error(&machine);
                break;
                
            default:
                TRANSITION(&machine, STATE_ERROR);
                break;
        }
    }
    
    return 0;
}