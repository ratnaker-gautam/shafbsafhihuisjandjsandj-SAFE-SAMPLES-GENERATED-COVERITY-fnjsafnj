//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char event;
    State next;
} Transition;

typedef struct {
    State state;
    char* buffer;
    size_t buffer_size;
    size_t data_length;
    int error_code;
} StateMachine;

Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'F', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'S', STATE_WRITING},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_WRITING, 'C', STATE_DONE},
    {STATE_WRITING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_DONE, 'R', STATE_IDLE}
};

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm);
        return NULL;
    }
    
    sm->state = STATE_IDLE;
    sm->buffer_size = BUFFER_SIZE;
    sm->data_length = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        if (sm->buffer) {
            free(sm->buffer);
        }
        free(sm);
    }
}

State get_next_state(State current, char event) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return transitions[i].next;
        }
    }
    return current;
}

int process_event(StateMachine* sm, char event) {
    if (!sm) return -1;
    
    State next_state = get_next_state(sm->state, event);
    
    switch (next_state) {
        case STATE_READING:
            if (sm->state == STATE_IDLE) {
                sm->data_length = 0;
                memset(sm->buffer, 0, sm->buffer_size);
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->state == STATE_READING && sm->data_length < sm->buffer_size) {
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
            }
            break;
            
        case STATE_WRITING:
            if (sm->state == STATE_PROCESSING && sm->data_length > 0) {
                printf("Processed data: %s\n", sm->buffer);
            }
            break;
            
        case STATE_ERROR:
            sm->error_code = 1;
            printf("State machine error occurred\n");
            break;
            
        case STATE_DONE:
            printf("Operation completed successfully\n");
            break;
            
        default:
            break;
    }
    
    sm->state = next_state;
    return 0;
}

int read_input_data(StateMachine* sm, const char* input) {
    if (!sm || !input || sm->state != STATE_READING) return -1;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return -1;
    
    if (sm->data_length + input_len >= sm->buffer_size) {
        return -1;
    }
    
    memcpy(sm->buffer + sm->data_length, input, input_len);
    sm->data_length += input_len;
    
    return 0;
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[128];
    char event;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("S: Start reading, F: Finish reading, E: Error, R: Reset, C: Complete, Q: Quit\n");
    printf("During READING state, enter text to process\n");
    
    while (1) {
        printf("Current state: %d | Enter event: ", sm->state);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (input_buffer[0] == 'Q' || input_buffer[0] == 'q') {
            break;
        }
        
        event = input_buffer[0];
        
        if (sm->state == STATE_READING && event != 'F' && event != 'E') {
            size_t len = strlen(input_buffer);
            if (len > 0 && input_buffer[len-1] == '\n') {
                input_buffer[len-1] = '\0';
            }
            if