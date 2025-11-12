//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_FINALIZE_COMPLETE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_length;
    int processing_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    machine->input_length = 0;
    machine->processing_count = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
}

Event get_next_event(StateMachine *machine) {
    if (machine == NULL) return EVENT_ERROR;
    
    char input_line[MAX_INPUT_LEN];
    if (fgets(input_line, MAX_INPUT_LEN, stdin) != NULL) {
        size_t len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
            len--;
        }
        
        if (len > 0 && len < MAX_INPUT_LEN) {
            strncpy(machine->input_buffer, input_line, len);
            *(machine->input_buffer + len) = '\0';
            machine->input_length = len;
            return EVENT_INPUT_RECEIVED;
        }
    }
    return EVENT_NONE;
}

void process_state_transition(StateMachine *machine, Event event) {
    if (machine == NULL) return;
    
    State *current = &(machine->current_state);
    
    switch (*current) {
        case STATE_START:
            if (event == EVENT_INPUT_RECEIVED) {
                *current = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_INPUT_RECEIVED) {
                *current = STATE_PROCESSING;
                machine->processing_count = 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                *current = STATE_FINALIZING;
            } else if (event == EVENT_INPUT_RECEIVED) {
                machine->processing_count++;
                if (machine->processing_count >= 3) {
                    *current = STATE_FINALIZING;
                }
            }
            break;
            
        case STATE_FINALIZING:
            if (event == EVENT_FINALIZE_COMPLETE) {
                *current = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            break;
    }
}

void execute_state_action(StateMachine *machine) {
    if (machine == NULL) return;
    
    char *buffer_ptr = machine->input_buffer;
    size_t length = machine->input_length;
    
    switch (machine->current_state) {
        case STATE_START:
            printf("State machine started. Enter text:\n");
            break;
            
        case STATE_READING:
            printf("Input received: %s\n", buffer_ptr);
            break;
            
        case STATE_PROCESSING:
            printf("Processing input (%zu chars): ", length);
            for (size_t i = 0; i < length && i < 20; i++) {
                printf("%c", *(buffer_ptr + i));
            }
            printf("\n");
            break;
            
        case STATE_FINALIZING:
            printf("Finalizing. Processed count: %d\n", machine->processing_count);
            break;
            
        case STATE_DONE:
            printf("State machine completed.\n");
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    int max_iterations = 100;
    int iteration = 0;
    
    while (machine.current_state != STATE_DONE && iteration < max_iterations) {
        Event event = get_next_event(&machine);
        
        if (event != EVENT_NONE && event != EVENT_ERROR) {
            process_state_transition(&machine, event);
            execute_state_action(&machine);
            
            if (machine.current_state == STATE_PROCESSING) {
                process_state_transition(&machine, EVENT_PROCESS_COMPLETE);
                execute_state_action(&machine);
            } else if (machine.current_state == STATE_FINALIZING) {
                process_state_transition(&machine, EVENT_FINALIZE_COMPLETE);
                execute_state_action(&machine);
            }
        }
        
        iteration++;
        
        if (machine.current_state == STATE_START) {
            process_state_transition(&machine, EVENT_INPUT_RECEIVED);
            execute_state_action(&machine);
        }
    }
    
    if (iteration >= max_iterations) {
        printf("Maximum iterations reached.\n");
    }
    
    return 0;
}