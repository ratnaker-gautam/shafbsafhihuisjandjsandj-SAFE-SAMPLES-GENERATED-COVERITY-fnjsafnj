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
#define MAX_DATA_LEN 50

typedef struct {
    int state;
    char data[MAX_DATA_LEN];
    size_t data_len;
} StateMachine;

int get_event(const char* input, size_t* pos) {
    if (*pos >= strlen(input)) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'R') return EVENT_RESET;
    if (isalnum(c)) return EVENT_DATA;
    return EVENT_INVALID;
}

void state_machine_init(StateMachine* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, MAX_DATA_LEN);
}

int state_machine_step(StateMachine* sm, int event) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data[sm->data_len++] = 'X';
                    sm->state = STATE_PROCESSING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
            } else if (event == EVENT_INVALID) {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                sm->state = STATE_READING;
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
            } else if (event == EVENT_INVALID) {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
    }
    
    return sm->state;
}

const char* state_to_string(int state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_DONE: return "DONE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(int event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA: return "DATA";
        case EVENT_END: return "END";
        case EVENT_RESET: return "RESET";
        case EVENT_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter sequence (S=start, R=reset, alphanum=data, end with newline): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_machine_init(&sm);
    
    printf("Initial state: %s\n", state_to_string(sm.state));
    
    size_t pos = 0;
    int event;
    
    while ((event = get_event(input, &pos)) != EVENT_END && sm.state != STATE_ERROR && sm.state != STATE_DONE) {
        int old_state = sm.state;
        state_machine_step(&sm, event);
        printf("Event: %s -> State: %s", event_to_string(event), state_to_string(sm.state));
        
        if (sm.state == STATE_PROCESSING && old_state == STATE_READING) {
            printf(" (processed data, length: %zu)", sm.data_len);
        }
        printf("\n");
    }
    
    if (event == EVENT_END && sm.state != STATE_DONE && sm.state != STATE_ERROR) {
        state_machine_step(&sm, EVENT_END);
        printf("Event: END -> State: %s\n", state_to_string(sm.state));
    }
    
    printf("Final state: %s\n", state_to_string(sm.state));
    
    if (sm.state == STATE_DONE) {
        printf("Processing complete. Data length: %zu\n", sm.data_len);
    } else if (sm.state == STATE_ERROR) {
        printf("Processing failed due to error\n");
    }
    
    return 0;
}