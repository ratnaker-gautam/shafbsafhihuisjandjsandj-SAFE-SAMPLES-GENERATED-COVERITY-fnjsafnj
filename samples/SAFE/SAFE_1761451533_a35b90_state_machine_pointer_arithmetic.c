//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define MAX_INPUT_LEN 100

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_END,
    EVENT_ERROR,
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int data_count;
} StateMachine;

State handle_event(StateMachine *sm, Event event, const char *data) {
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                sm->buffer_pos = 0;
                sm->data_count = 0;
                *(sm->buffer + sm->buffer_pos) = '\0';
                next_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA && data != NULL) {
                size_t data_len = strlen(data);
                if (sm->buffer_pos + data_len < MAX_INPUT_LEN - 1) {
                    char *dest = sm->buffer + sm->buffer_pos;
                    const char *src = data;
                    while (*src != '\0') {
                        *dest = *src;
                        dest++;
                        src++;
                    }
                    *dest = '\0';
                    sm->buffer_pos += data_len;
                    sm->data_count++;
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_DONE;
                }
            } else if (event == EVENT_END) {
                next_state = STATE_FINALIZING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                if (sm->data_count < 3) {
                    next_state = STATE_READING;
                } else {
                    next_state = STATE_FINALIZING;
                }
            } else if (event == EVENT_ERROR) {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_FINALIZING:
            if (event == EVENT_COMPLETE) {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return next_state;
}

void process_data(StateMachine *sm) {
    if (sm->buffer_pos > 0) {
        char *ptr = sm->buffer;
        int uppercase_count = 0;
        
        while (*ptr != '\0') {
            if (*ptr >= 'A' && *ptr <= 'Z') {
                uppercase_count++;
            }
            ptr++;
        }
        
        printf("Processed %zu characters, found %d uppercase letters\n", 
               sm->buffer_pos, uppercase_count);
        
        sm->buffer_pos = 0;
        *(sm->buffer) = '\0';
    }
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_pos = 0;
    sm.data_count = 0;
    
    char input[MAX_INPUT_LEN];
    
    sm.current_state = handle_event(&sm, EVENT_START, NULL);
    
    while (sm.current_state != STATE_DONE) {
        switch (sm.current_state) {
            case STATE_READING:
                printf("Enter data (or 'end' to finish): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    
                    if (strcmp(input, "end") == 0) {
                        sm.current_state = handle_event(&sm, EVENT_END, NULL);
                    } else if (strlen(input) > 0) {
                        sm.current_state = handle_event(&sm, EVENT_DATA, input);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                process_data(&sm);
                sm.current_state = handle_event(&sm, EVENT_COMPLETE, NULL);
                break;
                
            case STATE_FINALIZING:
                if (sm.buffer_pos > 0) {
                    process_data(&sm);
                }
                printf("Processing complete. Total data chunks: %d\n", sm.data_count);
                sm.current_state = handle_event(&sm, EVENT_COMPLETE, NULL);
                break;
                
            default:
                sm.current_state = STATE_DONE;
                break;
        }
    }
    
    printf("State machine finished.\n");
    return 0;
}