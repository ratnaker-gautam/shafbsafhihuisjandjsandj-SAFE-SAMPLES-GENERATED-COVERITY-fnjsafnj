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
    EVENT_DONE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(StateMachine *sm, Event event, const char *data);
void initialize_machine(StateMachine *sm);
int validate_input(const char *input);
void process_data(StateMachine *sm);

State handle_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->data_count = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (data == NULL) return STATE_ERROR;
                size_t data_len = strlen(data);
                if (data_len == 0 || data_len >= MAX_INPUT_LEN) return STATE_ERROR;
                
                if (sm->buffer_len + data_len < MAX_INPUT_LEN) {
                    strncpy(sm->buffer + sm->buffer_len, data, data_len);
                    sm->buffer_len += data_len;
                    sm->buffer[sm->buffer_len] = '\0';
                    sm->data_count++;
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_PROCESS && sm->data_count > 0) {
                return STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DONE) {
                process_data(sm);
                return STATE_FINISHED;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_START) {
                initialize_machine(sm);
                return STATE_READING;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_START) {
                initialize_machine(sm);
                return STATE_READING;
            }
            break;
            
        default:
            break;
    }
    
    return sm->current_state;
}

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_data(StateMachine *sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    printf("Processing %d data items:\n", sm->data_count);
    printf("Buffer content: %s\n", sm->buffer);
    printf("Buffer length: %zu\n", sm->buffer_len);
    
    int char_count = 0;
    int digit_count = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= '0' && sm->buffer[i] <= '9') {
            digit_count++;
        } else if ((sm->buffer[i] >= 'A' && sm->buffer[i] <= 'Z') || 
                  (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z')) {
            char_count++;
        }
    }
    
    printf("Letters: %d\n", char_count);
    printf("Digits: %d\n", digit_count);
    printf("Processing complete.\n");
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: data <text>, process, reset, quit\n");
    
    while (running) {
        printf("\nCurrent state: ");
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
        } else if (strncmp(input, "data ", 5) == 0) {
            const char *data = input + 5;
            if (validate_input