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
    EVENT_RESET,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int data_count;
} StateMachine;

Event get_next_event(const char *input, size_t *pos) {
    if (*pos >= strlen(input)) {
        return EVENT_INVALID;
    }
    
    char c = *(input + *pos);
    *pos += 1;
    
    switch (c) {
        case 'S': return EVENT_START;
        case 'D': return EVENT_DATA;
        case 'E': return EVENT_END;
        case 'R': return EVENT_RESET;
        default: return EVENT_INVALID;
    }
}

void state_machine_init(StateMachine *sm) {
    sm->current_state = STATE_START;
    sm->buffer_pos = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
}

int process_event(StateMachine *sm, Event event) {
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                next_state = STATE_READING;
                sm->buffer_pos = 0;
                sm->data_count = 0;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
                    *(sm->buffer + sm->buffer_pos) = 'D';
                    sm->buffer_pos++;
                    sm->data_count++;
                }
            } else if (event == EVENT_END) {
                next_state = STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_START) {
                next_state = STATE_FINALIZING;
            }
            break;
            
        case STATE_FINALIZING:
            if (event == EVENT_END) {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                next_state = STATE_START;
                sm->buffer_pos = 0;
                sm->data_count = 0;
                memset(sm->buffer, 0, MAX_INPUT_LEN);
            }
            break;
    }
    
    sm->current_state = next_state;
    return (event != EVENT_INVALID);
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter event sequence (S=Start, D=Data, E=End, R=Reset): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(input + input_len - 1) == '\n') {
        *(input + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    size_t pos = 0;
    int valid_events = 0;
    
    while (pos < input_len) {
        Event event = get_next_event(input, &pos);
        if (!process_event(&sm, event)) {
            fprintf(stderr, "Invalid event at position %zu\n", pos);
            return 1;
        }
        valid_events++;
        
        printf("State: %d, Data count: %d\n", sm.current_state, sm.data_count);
        
        if (sm.current_state == STATE_DONE) {
            printf("Processing complete. Buffer: %s\n", sm.buffer);
            break;
        }
    }
    
    if (sm.current_state != STATE_DONE) {
        printf("Sequence incomplete. Final state: %d\n", sm.current_state);
    }
    
    return 0;
}