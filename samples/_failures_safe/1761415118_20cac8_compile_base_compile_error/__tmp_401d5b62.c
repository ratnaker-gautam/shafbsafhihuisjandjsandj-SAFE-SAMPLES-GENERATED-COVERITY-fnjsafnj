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
    
    if (strncmp(input, "START", 5) == 0) {
        return EVENT_START;
    } else if (strncmp(input, "DATA", 4) == 0) {
        return EVENT_DATA;
    } else if (strncmp(input, "END", 3) == 0) {
        return EVENT_END;
    } else {
        return EVENT_INVALID;
    }
}

int process_data(StateMachine* sm, const char* data, size_t len) {
    if (sm == NULL || data == NULL || len == 0) {
        return 0;
    }
    
    if (sm->buffer_len + len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_len, data, len);
    sm->buffer_len += len;
    sm->data_count++;
    
    return 1;
}

State handle_event(StateMachine* sm, Event event, const char* data, size_t len) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->buffer_len = 0;
                sm->data_count = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (process_data(sm, data, len)) {
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                return STATE_FINISHED;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                if (process_data(sm, data, len)) {
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                return STATE_FINISHED;
            }
            break;
            
        case STATE_FINISHED:
        case STATE_ERROR:
            break;
    }
    
    return STATE_ERROR;
}

void print_results(const StateMachine* sm) {
    if (sm == NULL) {
        return;
    }
    
    switch (sm->current_state) {
        case STATE_FINISHED:
            printf("Processing completed successfully.\n");
            printf("Data items processed: %d\n", sm->data_count);
            printf("Total data length: %zu\n", sm->buffer_len);
            if (sm->buffer_len > 0) {
                printf("Buffer contents: %s\n", sm->buffer);
            }
            break;
            
        case STATE_ERROR:
            printf("Error occurred during processing.\n");
            break;
            
        default:
            printf("Processing not completed. Current state: %d\n", sm->current_state);
            break;
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
    printf("Commands: START, DATA <text>, END\n");
    printf("Enter commands one per line:\n");
    
    while (running) {
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
        
        Event event = get_next_event(input, len);
        const char* data = NULL;
        size_t data_len = 0;
        
        if (event == EVENT_DATA) {
            data = input + 5;
            while (*data == ' ') data++;
            data_len = strlen(data);
            if (data_len == 0) {
                event = EVENT_INVALID;
            }
        }
        
        State new_state = handle_event(sm, event, data, data_len);