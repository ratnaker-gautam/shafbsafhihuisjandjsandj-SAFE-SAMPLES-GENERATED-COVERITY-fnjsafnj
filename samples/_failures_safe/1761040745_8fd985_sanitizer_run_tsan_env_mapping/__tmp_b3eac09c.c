//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
                if (data_len > 0 && sm->buffer_pos + data_len < MAX_INPUT_LEN - 1) {
                    char *dest = sm->buffer + sm->buffer_pos;
                    strncpy(dest, data, data_len);
                    *(dest + data_len) = '\0';
                    sm->buffer_pos += data_len;
                    sm->data_count++;
                }
                next_state = STATE_PROCESSING;
            } else if (event == EVENT_END) {
                next_state = STATE_FINALIZING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                next_state = STATE_READING;
            } else if (event == EVENT_END) {
                next_state = STATE_FINALIZING;
            }
            break;
            
        case STATE_FINALIZING:
            if (event == EVENT_START) {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            break;
    }
    
    return next_state;
}

Event get_next_event(const char *input) {
    if (input == NULL) return EVENT_ERROR;
    
    size_t len = strlen(input);
    if (len == 0) return EVENT_ERROR;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        if (*ptr == 'S') return EVENT_START;
        if (*ptr == 'D') return EVENT_DATA;
        if (*ptr == 'E') return EVENT_END;
        ptr++;
    }
    
    return EVENT_ERROR;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_START;
    sm.buffer_pos = 0;
    sm.data_count = 0;
    
    char input[MAX_INPUT_LEN];
    
    printf("Enter state machine commands (S=Start, D=Data, E=End): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    char *token = strtok(input, " ");
    while (token != NULL) {
        Event event = get_next_event(token);
        
        const char *data = NULL;
        if (event == EVENT_DATA) {
            data = "sample";
        }
        
        State new_state = handle_event(&sm, event, data);
        
        if (new_state != sm.current_state) {
            printf("Transition: %d -> %d\n", sm.current_state, new_state);
            sm.current_state = new_state;
        }
        
        token = strtok(NULL, " ");
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("Processing complete. Data count: %d\n", sm.data_count);
        if (sm.buffer_pos > 0) {
            printf("Buffer content: %s\n", sm.buffer);
        }
    } else {
        printf("Processing incomplete. Final state: %d\n", sm.current_state);
    }
    
    return EXIT_SUCCESS;
}