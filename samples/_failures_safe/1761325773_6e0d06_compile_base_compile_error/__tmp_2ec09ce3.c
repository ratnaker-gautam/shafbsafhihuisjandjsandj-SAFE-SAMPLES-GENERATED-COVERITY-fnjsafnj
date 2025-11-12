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
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(StateMachine *sm, Event event, const char *data);
void process_data(StateMachine *sm);
int validate_input(const char *input, size_t len);
void initialize_machine(StateMachine *sm);
void cleanup_machine(StateMachine *sm);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

void cleanup_machine(StateMachine *sm) {
    if (sm == NULL) return;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_data(StateMachine *sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    int sum = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] == '\0') break;
        sum += (int)sm->buffer[i];
    }
    
    if (sum < 0) {
        sum = 0;
    }
    
    printf("Processed data sum: %d\n", sum);
    sm->data_count++;
}

State handle_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("State machine started\n");
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data, strlen(data))) {
                        size_t data_len = strlen(data);
                        if (data_len < sizeof(sm->buffer)) {
                            strncpy(sm->buffer, data, sizeof(sm->buffer) - 1);
                            sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                            sm->buffer_len = data_len;
                            printf("Data received: %s\n", sm->buffer);
                            return STATE_PROCESSING;
                        }
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return sm->current_state;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(sm);
                    return STATE_FINISHED;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return sm->current_state;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_COMPLETE:
                    printf("Processing complete. Total data processed: %d\n", sm->data_count);
                    return STATE_START;
                case EVENT_START:
                    return STATE_READING;
                default:
                    return sm->current_state;
            }
            
        case STATE_ERROR:
            printf("Error state reached\n");
            return STATE_START;
            
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, process, complete, quit\n");
    
    while (running) {
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\n> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strcmp(input, "start") == 0) {
            sm.current_state = handle_event(&sm, EVENT_START, NULL);
        } else if (