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
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_event(State current, Event event, StateMachine* sm);
int validate_input(const char* input, size_t len);
void initialize_machine(StateMachine* sm);
void process_data(StateMachine* sm);
void print_state_info(const StateMachine* sm);

State handle_event(State current, Event event, StateMachine* sm) {
    switch(current) {
        case STATE_START:
            if (event == EVENT_START) {
                return STATE_READING;
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->buffer_len > 0) {
                    return STATE_PROCESSING;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS) {
                process_data(sm);
                return STATE_FINISHED;
            }
            break;
        case STATE_FINISHED:
            if (event == EVENT_COMPLETE) {
                return STATE_START;
            }
            break;
        default:
            break;
    }
    return STATE_ERROR;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void initialize_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    int word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] == ' ' || sm->buffer[i] == '\t' || sm->buffer[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }
    
    sm->processed_count += word_count;
    printf("Processed %d words from input. Total: %d\n", word_count, sm->processed_count);
}

void print_state_info(const StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Buffer: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    printf("Total words processed: %d\n\n", sm->processed_count);
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&sm);
    
    printf("State Machine Demo - Word Counter\n");
    printf("Enter text lines (empty line to exit):\n");
    
    while (1) {
        print_state_info(&sm);
        
        if (sm.current_state == STATE_START) {
            printf("Starting machine...\n");
            sm.current_state = handle_event(sm.current_state, EVENT_START, &sm);
            continue;
        }
        
        if (sm.current_state == STATE_READING) {
            printf("Enter text: ");
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                sm.current_state = STATE_ERROR;
                break;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len-1] == '\n') {
                input[len-1] = '\0';
                len--;
            }
            
            if (len == 0) {
                printf("Empty line detected. Exiting.\n");
                break;
            }
            
            if (!validate_input(input, len)) {
                printf("Invalid input detected.\n");
                sm.current_state = STATE_ERROR;
                break;
            }
            
            if (len < sizeof(sm.buffer)) {
                memcpy(sm.buffer, input, len);
                sm.buffer[len] = '\0';
                sm.buffer_len = len;
                sm.current_state = handle_event(sm.current_state, EVENT_DATA, &sm);
            } else {
                printf("Input too long.\n");
                sm.current_state = STATE_ERROR;
                break;
            }
        }
        
        if (sm.current_state == STATE_PROCESSING) {
            sm.current_state = handle_event(sm.current_state, EVENT_PROCESS, &sm);
        }
        
        if (sm.current_state