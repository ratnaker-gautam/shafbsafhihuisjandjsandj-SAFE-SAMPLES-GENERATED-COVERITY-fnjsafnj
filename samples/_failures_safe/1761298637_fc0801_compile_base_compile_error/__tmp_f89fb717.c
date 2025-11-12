//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 50

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_pos;
    int data_count;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->data_count = 0;
}

int get_next_event(const char *input, size_t *pos) {
    if (input == NULL || pos == NULL) {
        return EVENT_INVALID;
    }
    
    size_t input_len = strlen(input);
    if (*pos >= input_len) {
        return EVENT_END;
    }
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'D') return EVENT_DATA;
    if (c == 'E') return EVENT_END;
    if (c == 'R') return EVENT_RESET;
    
    return EVENT_INVALID;
}

int state_machine_step(StateMachine *sm, int event, char data) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    int next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->buffer_pos = 0;
                sm->data_count = 0;
                next_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->buffer_pos < MAX_BUFFER - 1) {
                    sm->buffer[sm->buffer_pos++] = data;
                    sm->data_count++;
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                next_state = STATE_DONE;
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            } else if (event == EVENT_INVALID) {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                if (sm->buffer_pos < MAX_BUFFER - 1) {
                    sm->buffer[sm->buffer_pos++] = data;
                    sm->data_count++;
                    next_state = STATE_READING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                next_state = STATE_DONE;
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            } else if (event == EVENT_INVALID) {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
            }
            break;
    }
    
    return next_state;
}

void process_buffer(StateMachine *sm) {
    if (sm == NULL || sm->buffer_pos == 0) {
        return;
    }
    
    printf("Processed: ");
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isalnum((unsigned char)sm->buffer[i])) {
            putchar(sm->buffer[i]);
        }
    }
    printf(" (count: %d)\n", sm->data_count);
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    printf("Enter sequence (S=Start, D=Data, E=End, R=Reset, followed by char for D): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t pos = 0;
    char data_char = ' ';
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        int event = get_next_event(input, &pos);
        
        if (event == EVENT_DATA && pos < strlen(input)) {
            data_char = input[pos];
            pos++;
        } else {
            data_char = ' ';
        }
        
        int new_state = state_machine_step(&sm, event, data_char);
        
        if (new_state != sm.current_state) {
            printf("State: %d -> %d (Event: %d)\n", sm.current_state, new_state, event