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
    char buffer[MAX_INPUT_LEN];
    size_t position;
    size_t length;
    State current_state;
    int processed_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    memset(machine->buffer, 0, MAX_INPUT_LEN);
    machine->position = 0;
    machine->length = 0;
    machine->current_state = STATE_IDLE;
    machine->processed_count = 0;
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

State get_next_state(State current, char input) {
    static const Transition transitions[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_IDLE, 'Q', STATE_COMPLETE},
        {STATE_READING, 'E', STATE_PROCESSING},
        {STATE_READING, 'Q', STATE_COMPLETE},
        {STATE_PROCESSING, 'C', STATE_IDLE},
        {STATE_PROCESSING, 'Q', STATE_COMPLETE}
    };
    
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    
    return STATE_ERROR;
}

void process_buffer(StateMachine *machine) {
    if (machine->length == 0) return;
    
    for (size_t i = 0; i < machine->length; i++) {
        if (machine->buffer[i] != '\0') {
            machine->buffer[i] = toupper(machine->buffer[i]);
        }
    }
    
    machine->processed_count++;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    char input;
    int running = 1;
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, E=End input, C=Continue, Q=Quit\n");
    printf("Enter alphanumeric data when in READING state\n");
    
    while (running) {
        printf("\nCurrent state: ");
        switch (machine.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Processed: %d\n", machine.processed_count);
        
        if (machine.current_state == STATE_COMPLETE) {
            break;
        }
        
        if (machine.current_state == STATE_READING) {
            printf("Enter data character: ");
        } else {
            printf("Enter command: ");
        }
        
        int result = scanf(" %c", &input);
        if (result != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        State next_state = get_next_state(machine.current_state, input);
        
        switch (next_state) {
            case STATE_READING:
                if (machine.current_state == STATE_IDLE) {
                    memset(machine.buffer, 0, MAX_INPUT_LEN);
                    machine.position = 0;
                    machine.length = 0;
                }
                machine.current_state = STATE_READING;
                break;
                
            case STATE_PROCESSING:
                if (machine.current_state == STATE_READING) {
                    machine.current_state = STATE_PROCESSING;
                    process_buffer(&machine);
                    printf("Processed: %.*s\n", (int)machine.length, machine.buffer);
                } else {
                    machine.current_state = STATE_ERROR;
                }
                break;
                
            case STATE_IDLE:
                if (machine.current_state == STATE_PROCESSING) {
                    machine.current_state = STATE_IDLE;
                } else {
                    machine.current_state = STATE_ERROR;
                }
                break;
                
            case STATE_COMPLETE:
                machine.current_state = STATE_COMPLETE;
                running = 0;
                break;
                
            case STATE_ERROR:
                machine.current_state = STATE_ERROR;
                printf("Invalid transition\n");
                break;
        }
        
        if (machine.current_state == STATE_READING && is_valid_input(input)) {
            if (machine.position < MAX_INPUT_LEN - 1) {
                machine.buffer[machine.position++] = input;
                machine.length = machine.position;
            } else {
                printf("Buffer full\n");
                machine.current_state = STATE_ERROR;
            }
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("Resetting to IDLE state\n");
            machine.current_state = STATE_IDLE;
        }
    }
    
    printf("Final processed count: %d\n", machine.processed_count);
    return 0;
}