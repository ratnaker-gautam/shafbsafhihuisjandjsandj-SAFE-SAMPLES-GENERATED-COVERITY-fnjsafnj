//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

State handle_event(StateMachine* sm, Event event, const char* data);
void initialize_machine(StateMachine* sm);
int validate_input(const char* input);
Event get_next_event(void);

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    printf("State Machine Demo - Enter commands:\n");
    printf("start, data <text>, process, complete, error, reset, quit\n");
    
    char input[MAX_INPUT_LEN];
    
    while (sm.current_state != STATE_FINISHED) {
        printf("Current state: %d > ", sm.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event;
        char event_data[MAX_INPUT_LEN] = {0};
        
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strncmp(input, "data ", 5) == 0) {
            event = EVENT_DATA;
            const char* data_start = input + 5;
            if (strlen(data_start) > 0) {
                strncpy(event_data, data_start, sizeof(event_data) - 1);
                event_data[sizeof(event_data) - 1] = '\0';
            }
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "complete") == 0) {
            event = EVENT_COMPLETE;
        } else if (strcmp(input, "error") == 0) {
            event = EVENT_ERROR;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;
        } else {
            printf("Unknown command\n");
            continue;
        }
        
        State new_state = handle_event(&sm, event, event_data);
        sm.current_state = new_state;
        
        if (sm.current_state == STATE_FINISHED) {
            printf("Processing complete. Data count: %d\n", sm.data_count);
        } else if (sm.current_state == STATE_ERROR) {
            printf("Error state reached\n");
        }
    }
    
    return 0;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_START;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("Machine started\n");
                    return STATE_READING;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    printf("Invalid event for START state\n");
                    return STATE_START;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && strlen(data) > 0) {
                        if (sm->buffer_len + strlen(data) < MAX_INPUT_LEN) {
                            strcat(sm->buffer, data);
                            sm->buffer_len = strlen(sm->buffer);
                            sm->data_count++;
                            printf("Data added: %s (total: %zu chars)\n", data, sm->buffer_len);
                        } else {
                            printf("Buffer full\n");
                        }
                    }
                    return STATE_READING;
                case EVENT_PROCESS:
                    if (sm->buffer_len > 0) {
                        printf("Processing %zu characters\n", sm->buffer_len);
                        return STATE_PROCESSING;
                    } else {
                        printf("No data to process\n");
                        return STATE_READING;
                    }
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    printf("Invalid event for READING state\n");
                    return STATE_READING;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    printf("Processing complete\n");
                    return STATE_FINISHED;
                case EVENT_ERROR:
                    printf("Processing error\n");
                    return STATE_ERROR;
                case EVENT_RESET:
                    initialize_machine(sm);
                    return STATE_START;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    return STATE_PROCESSING;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case