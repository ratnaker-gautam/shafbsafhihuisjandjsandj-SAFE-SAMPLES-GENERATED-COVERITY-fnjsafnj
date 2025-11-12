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
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int data_count;
} StateMachine;

State handle_event(StateMachine* sm, Event event, const char* data);
void process_data(const char* data);
int validate_input(const char* input);
void print_state(State state);

State handle_event(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    memset(sm->buffer, 0, MAX_INPUT_LEN);
                    sm->buffer_pos = 0;
                    sm->data_count = 0;
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (data != NULL && validate_input(data)) {
                        size_t data_len = strlen(data);
                        if (sm->buffer_pos + data_len < MAX_INPUT_LEN - 1) {
                            strncpy(sm->buffer + sm->buffer_pos, data, data_len);
                            sm->buffer_pos += data_len;
                            sm->data_count++;
                            return STATE_READING;
                        }
                    }
                    return STATE_ERROR;
                    
                case EVENT_COMPLETE:
                    if (sm->data_count > 0) {
                        return STATE_PROCESSING;
                    }
                    return STATE_ERROR;
                    
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_START:
                    process_data(sm->buffer);
                    return STATE_FINISHED;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_FINISHED;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

void process_data(const char* data) {
    if (data == NULL) return;
    
    size_t len = strlen(data);
    printf("Processing %zu characters: ", len);
    
    for (size_t i = 0; i < len && i < 50; i++) {
        if (data[i] >= 32 && data[i] <= 126) {
            putchar(data[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    
    return 1;
}

void print_state(State state) {
    const char* state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    StateMachine sm = {STATE_START, {0}, 0, 0};
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("start, data <text>, complete, reset, quit\n");
    
    while (running) {
        print_state(sm.current_state);
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
        } else if (strcmp(input, "start") == 0) {
            sm.current_state = handle_event(&sm, EVENT_START, NULL);
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* text = input + 5;
            if (validate_input(text)) {
                sm.current_state = handle_event(&sm, EVENT_DATA, text);
            } else {
                printf("Invalid input data\n");
            }
        } else if (strcmp(input, "complete") == 0) {
            sm.current_state = handle_event(&sm, EVENT_COMPLETE, NULL);
        } else if (strcmp(input, "reset") == 0) {
            sm.current_state = handle_event(&sm, EVENT_RESET, NULL);
        } else {
            printf("Unknown command\n");
        }
        
        if (sm.current_state == STATE_ERROR) {
            printf("State