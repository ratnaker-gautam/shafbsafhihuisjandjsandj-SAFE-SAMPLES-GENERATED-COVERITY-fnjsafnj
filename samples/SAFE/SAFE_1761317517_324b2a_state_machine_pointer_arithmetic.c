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
    EVENT_ERROR
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
                next_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA && data != NULL) {
                size_t data_len = strlen(data);
                if (data_len > 0 && sm->buffer_pos + data_len < MAX_INPUT_LEN) {
                    char *dest = sm->buffer + sm->buffer_pos;
                    const char *src = data;
                    while (*src && (dest - sm->buffer) < MAX_INPUT_LEN - 1) {
                        *dest++ = *src++;
                    }
                    sm->buffer_pos = dest - sm->buffer;
                    sm->data_count++;
                    next_state = STATE_PROCESSING;
                }
            } else if (event == EVENT_END) {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA || event == EVENT_END) {
                if (sm->buffer_pos > 0) {
                    char *start = sm->buffer;
                    char *end = sm->buffer + sm->buffer_pos - 1;
                    while (start < end) {
                        char temp = *start;
                        *start = *end;
                        *end = temp;
                        start++;
                        end--;
                    }
                    next_state = STATE_FINALIZING;
                }
            }
            break;
            
        case STATE_FINALIZING:
            if (sm->buffer_pos > 0 && sm->buffer_pos < MAX_INPUT_LEN) {
                sm->buffer[sm->buffer_pos] = '\0';
                printf("Processed data %d: %s\n", sm->data_count, sm->buffer);
                sm->buffer_pos = 0;
                next_state = STATE_READING;
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return next_state;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_pos = 0;
    sm.data_count = 0;
    
    char input[MAX_INPUT_LEN];
    
    sm.current_state = handle_event(&sm, EVENT_START, NULL);
    
    printf("Enter data lines (empty line to finish):\n");
    
    while (sm.current_state != STATE_DONE) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        Event event;
        if (len == 0) {
            event = EVENT_END;
        } else {
            event = EVENT_DATA;
        }
        
        State new_state = handle_event(&sm, event, len > 0 ? input : NULL);
        
        if (new_state == sm.current_state && event == EVENT_DATA) {
            printf("Invalid transition, ignoring input\n");
        }
        
        sm.current_state = new_state;
    }
    
    printf("Processing complete. Total data items: %d\n", sm.data_count);
    
    return 0;
}