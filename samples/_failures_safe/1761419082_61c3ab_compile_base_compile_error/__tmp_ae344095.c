//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_finished(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_start(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Enter data character: ");
                int c = getchar();
                if (c == EOF || c == '\n') {
                    return STATE_READING;
                }
                if (c >= 32 && c <= 126) {
                    sm->buffer[sm->buffer_len++] = (char)c;
                    sm->buffer[sm->buffer_len] = '\0';
                    sm->data_count++;
                    printf("Data stored: %c (count: %d)\n", (char)c, sm->data_count);
                }
                while ((c = getchar()) != '\n' && c != EOF);
            }
            if (sm->data_count >= 5) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: %s\n", sm->buffer);
            printf("Total characters processed: %d\n", sm->data_count);
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_finished(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete.\n");
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_START;
        default:
            printf("Error state. Use reset to continue.\n");
            return STATE_ERROR;
    }
}

void process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return;
    
    State (*handlers[])(StateMachine*, Event) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    if (sm->current_state < STATE_START || sm->current_state > STATE_ERROR) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    State new_state = handlers[sm->current_state](sm, event);
    if (new_state >= STATE_START && new_state <= STATE_ERROR) {
        sm->current_state = new_state;
    } else {
        sm->current_state = STATE_ERROR;
    }
}

Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("1 - Start\n");
    printf("2 - Data input\n");
    printf("3 - Process data\n");
    printf("4 - Complete\n");
    printf("5 - Reset\n");
    printf("6 - Exit\n");
    printf("Choose event (1-6): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || choice < 1 || choice > 6) {
        return EVENT_ERROR;
    }
    
    switch (choice) {
        case 1: return EVENT_START;
        case 2: return EVENT_DATA;
        case 3: return