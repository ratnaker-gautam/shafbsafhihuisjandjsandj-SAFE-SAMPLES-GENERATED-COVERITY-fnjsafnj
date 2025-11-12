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
    if (input == NULL || len == 0) {
        return EVENT_INVALID;
    }
    
    if (len == 1 && input[0] == 'S') {
        return EVENT_START;
    }
    
    if (len == 1 && input[0] == 'E') {
        return EVENT_END;
    }
    
    if (len > 0 && len < MAX_INPUT_LEN) {
        return EVENT_DATA;
    }
    
    return EVENT_INVALID;
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

int handle_data_event(StateMachine* sm, const char* data, size_t len) {
    if (sm == NULL || data == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    if (sm->current_state != STATE_READING) {
        return 0;
    }
    
    if (sm->buffer_len + len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_len, data, len);
    sm->buffer_len += len;
    sm->data_count++;
    sm->current_state = STATE_PROCESSING;
    printf("State: PROCESSING - Processed data chunk %d\n", sm->data_count);
    sm->current_state = STATE_READING;
    return 1;
}

int handle_end_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    
    if (sm->current_state != STATE_READING) {
        return 0;
    }
    
    sm->current_state = STATE_FINISHED;
    printf("State: FINISHED - Processed %d data chunks, total length: %zu\n", 
           sm->data_count, sm->buffer_len);
    return 1;
}

int handle_invalid_event(StateMachine* sm) {
    if (sm == NULL) {
        return 0;
    }
    sm->current_state = STATE_ERROR;
    printf("State: ERROR - Invalid event received\n");
    return 1;
}

int process_event(StateMachine* sm, Event event, const char* data, size_t len) {
    if (sm == NULL) {
        return 0;
    }
    
    switch (event) {
        case EVENT_START:
            return handle_start_event(sm);
        case EVENT_DATA:
            return handle_data_event(sm, data, len);
        case EVENT_END:
            return handle_end_event(sm);
        case EVENT_INVALID:
            return handle_invalid_event(sm);
        default:
            return 0;
    }
}

void print_usage(void) {
    printf("Commands:\n");
    printf("  S - Start processing\n");
    printf("  E - End processing\n");
    printf("  <text> - Data to process\n");
    printf("  Q - Quit\n");
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    print_usage();
    
    while (running) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
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
        
        if (len == 1 && input[0] == 'Q') {
            break;
        }
        
        Event event = get_next_event(input, len);
        const char* data = (event == EVENT_DATA) ? input : NULL;
        size_t data_len = (event == EVENT_DATA) ? len : 0;
        
        if (!process_event(sm,