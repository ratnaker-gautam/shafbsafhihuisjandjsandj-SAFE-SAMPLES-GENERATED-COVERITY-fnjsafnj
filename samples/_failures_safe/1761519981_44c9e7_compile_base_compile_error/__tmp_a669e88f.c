//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
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
    EVENT_END,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    sm->current_state = STATE_START;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
}

Event get_next_event(const char* input, size_t len) {
    if (len == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "START") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "DATA") == 0) {
        return EVENT_DATA;
    } else if (strcmp(input, "END") == 0) {
        return EVENT_END;
    } else {
        return EVENT_INVALID;
    }
}

int handle_start_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    sm->current_state = STATE_READING;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State: READING - Ready to receive data\n");
    return 1;
}

int handle_data_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    if (sm->current_state != STATE_READING) {
        return 0;
    }
    
    if (sm->data_count >= 5) {
        sm->current_state = STATE_PROCESSING;
        printf("State: PROCESSING - Processing %d data items\n", sm->data_count);
    } else {
        sm->data_count++;
        printf("State: READING - Received data item %d\n", sm->data_count);
    }
    return 1;
}

int handle_end_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    if (sm->current_state == STATE_READING || sm->current_state == STATE_PROCESSING) {
        sm->current_state = STATE_FINISHED;
        printf("State: FINISHED - Processed %d data items\n", sm->data_count);
        return 1;
    }
    return 0;
}

int handle_invalid_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    sm->current_state = STATE_ERROR;
    printf("State: ERROR - Invalid event received\n");
    return 1;
}

int process_event(StateMachine* sm, Event event) {
    if (sm == NULL) {
        return 0;
    }
    
    switch (event) {
        case EVENT_START:
            return handle_start_event(sm);
        case EVENT_DATA:
            return handle_data_event(sm);
        case EVENT_END:
            return handle_end_event(sm);
        case EVENT_INVALID:
            return handle_invalid_event(sm);
        default:
            return 0;
    }
}

void print_state_machine_status(const StateMachine* sm) {
    if (sm == NULL) {
        return;
    }
    
    const char* state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (sm->current_state < sizeof(state_names)/sizeof(state_names[0])) {
        printf("Current state: %s\n", state_names[sm->current_state]);
    }
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Available commands: START, DATA, END, QUIT\n");
    
    while (running) {
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input, "QUIT") == 0) {
            running = 0;
            continue;
        }
        
        Event event = get_next_event(input, len);
        if (!process_event(sm, event)) {
            printf("Failed to process event\n");
        }
        
        if (sm->current_state == STATE_FINISHED) {
            printf("Processing complete. Enter START to begin again or QUIT to