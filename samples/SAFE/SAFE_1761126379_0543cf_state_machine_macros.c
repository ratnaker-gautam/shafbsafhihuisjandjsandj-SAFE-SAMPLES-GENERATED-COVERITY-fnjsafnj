//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int data_len;
} state_machine_t;

int get_next_event(const char* input, int* pos) {
    if (*pos >= (int)strlen(input)) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'R') return EVENT_RESET;
    if (isalnum(c)) return EVENT_DATA;
    
    return EVENT_INVALID;
}

void state_machine_init(state_machine_t* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, sizeof(sm->data));
}

int state_machine_step(state_machine_t* sm, int event, char data_char) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data[sm->data_len++] = data_char;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else {
                sm->state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        default:
            sm->state = STATE_ERROR;
            break;
    }
    
    return sm->state;
}

void process_data(const state_machine_t* sm) {
    if (sm->state == STATE_PROCESSING && sm->data_len > 0) {
        printf("Processing data: ");
        for (int i = 0; i < sm->data_len; i++) {
            printf("%c", sm->data[i]);
        }
        printf("\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    state_machine_t sm;
    
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
    
    int pos = 0;
    int event;
    
    while ((event = get_next_event(input, &pos)) != EVENT_END && sm.state != STATE_ERROR) {
        char data_char = (event == EVENT_DATA && pos > 0) ? input[pos - 1] : '\0';
        int new_state = state_machine_step(&sm, event, data_char);
        
        if (new_state == STATE_PROCESSING) {
            process_data(&sm);
            state_machine_step(&sm, EVENT_INVALID, '\0');
        }
        
        if (sm.state == STATE_ERROR) {
            printf("State machine entered error state\n");
            break;
        }
    }
    
    if (sm.state == STATE_DONE) {
        printf("Processing completed successfully\n");
    } else if (sm.state != STATE_ERROR) {
        printf("Processing terminated in state: %d\n", sm.state);
    }
    
    return 0;
}