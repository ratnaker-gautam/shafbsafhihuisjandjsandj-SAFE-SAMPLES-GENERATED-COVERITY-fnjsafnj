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
    STATE_DONE,
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

State get_next_state(State current, Event event);
Event get_event(const char* input);
void handle_state_action(StateMachine* sm, Event event);
int is_valid_input(const char* input);

int main(void) {
    StateMachine sm = {STATE_START, {0}, 0, 0};
    char input[MAX_INPUT_LEN + 2];
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, end, quit\n");
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        printf("Current state: %d > ", sm.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) >= MAX_INPUT_LEN) {
            printf("Input too long\n");
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        Event event = get_event(input);
        State next_state = get_next_state(sm.current_state, event);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid transition from state %d with event %d\n", 
                   sm.current_state, event);
            sm.current_state = STATE_ERROR;
        } else {
            handle_state_action(&sm, event);
            sm.current_state = next_state;
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("Processing complete. Processed %d data items.\n", sm.data_count);
    } else if (sm.current_state == STATE_ERROR) {
        printf("State machine entered error state.\n");
    }
    
    return 0;
}

State get_next_state(State current, Event event) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_START) return STATE_READING;
            break;
        case STATE_READING:
            if (event == EVENT_DATA) return STATE_READING;
            if (event == EVENT_END) return STATE_PROCESSING;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_START) return STATE_READING;
            if (event == EVENT_END) return STATE_DONE;
            break;
        case STATE_DONE:
        case STATE_ERROR:
            break;
    }
    return STATE_ERROR;
}

Event get_event(const char* input) {
    if (input == NULL) return EVENT_INVALID;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "end") == 0) return EVENT_END;
    
    if (strncmp(input, "data ", 5) == 0) {
        const char* data_part = input + 5;
        if (strlen(data_part) > 0 && strlen(data_part) <= MAX_INPUT_LEN - 5) {
            return EVENT_DATA;
        }
    }
    
    return EVENT_INVALID;
}

void handle_state_action(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_START:
            memset(sm->buffer, 0, sizeof(sm->buffer));
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("Machine started\n");
            break;
            
        case EVENT_DATA:
            {
                const char* data = "data ";
                const char* input = data + 5;
                size_t data_len = strlen(input);
                if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                    memcpy(sm->buffer + sm->buffer_len, input, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    printf("Data added: %s (total: %zu chars, %d items)\n", 
                           input, sm->buffer_len, sm->data_count);
                }
            }
            break;
            
        case EVENT_END:
            printf("End command received. Buffer content: %.*s\n", 
                   (int)sm->buffer_len, sm->buffer);
            break;
            
        case EVENT_INVALID:
            break;
    }
}

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    return len > 0 && len < MAX_INPUT_LEN;
}