//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_len;
    int processed_count;
    int error_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, 'A', STATE_READING},
    {STATE_READING, 'B', STATE_READING},
    {STATE_READING, 'C', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->processed_count = 0;
    machine->error_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_input(StateMachine *machine, char input) {
    if (machine->current_state == STATE_COMPLETE || machine->current_state == STATE_ERROR) {
        return;
    }

    if (machine->input_len >= MAX_INPUT_LEN - 1) {
        machine->current_state = STATE_ERROR;
        machine->error_count++;
        return;
    }

    State next_state = get_next_state(machine->current_state, input);
    
    if (next_state == STATE_ERROR) {
        machine->current_state = STATE_ERROR;
        machine->error_count++;
        return;
    }

    if (machine->current_state == STATE_READING && next_state == STATE_READING) {
        if (machine->input_len < MAX_INPUT_LEN - 1) {
            machine->input_buffer[machine->input_len++] = input;
        }
    }

    if (next_state == STATE_PROCESSING) {
        if (machine->input_len > 0) {
            for (size_t i = 0; i < machine->input_len; i++) {
                if (machine->input_buffer[i] == 'A') {
                    machine->processed_count++;
                }
            }
            machine->input_len = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
        }
        next_state = STATE_IDLE;
    }

    machine->current_state = next_state;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, A/B/C=Data, E=Process, Q=Quit\n");
    printf("Enter command sequence: ");
    
    char input_line[MAX_INPUT_LEN * 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t line_len = strlen(input_line);
    if (line_len == 0) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < line_len; i++) {
        char ch = toupper((unsigned char)input_line[i]);
        if (ch == '\n' || ch == '\r') {
            continue;
        }
        
        process_input(&machine, ch);
        
        if (machine.current_state == STATE_COMPLETE) {
            break;
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("Error: Invalid transition at position %zu\n", i + 1);
            break;
        }
    }
    
    if (machine.current_state != STATE_COMPLETE && machine.current_state != STATE_ERROR) {
        printf("Warning: Incomplete sequence\n");
    }
    
    printf("Final state: ");
    switch (machine.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\n");
    
    printf("Processed items: %d\n", machine.processed_count);
    printf("Errors: %d\n", machine.error_count);
    
    return EXIT_SUCCESS;
}