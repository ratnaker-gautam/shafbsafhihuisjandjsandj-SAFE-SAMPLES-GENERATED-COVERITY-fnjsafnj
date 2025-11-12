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
    if (input == NULL || pos == NULL) return EVENT_INVALID;
    size_t len = strlen(input);
    if (*pos >= len) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'R') return EVENT_RESET;
    if (isalnum(c)) return EVENT_DATA;
    return EVENT_INVALID;
}

void state_machine_init(StateMachine* sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, MAX_DATA_LEN);
}

int state_machine_step(StateMachine* sm, int event, char data_char) {
    if (sm == NULL) return STATE_ERROR;
    
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
                    sm->data[sm->data_len++] = data_char;
                    sm->state = STATE_PROCESSING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data[sm->data_len++] = data_char;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
    }
    
    return sm->state;
}

void print_state_name(int state) {
    switch (state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_DONE: printf("DONE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_machine_init(&sm);
    
    printf("Initial state: ");
    print_state_name(sm.state);
    printf("\n");
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    while (sm.state != STATE_DONE && sm.state != STATE_ERROR && pos < input_len) {
        int event = get_event(input, &pos);
        char data_char = (event == EVENT_DATA && pos > 0) ? input[pos-1] : '\0';
        
        int old_state = sm.state;
        state_machine_step(&sm, event, data_char);
        
        printf("Event: ");
        switch (event) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA: printf("DATA('%c')", data_char); break;
            case EVENT_END: printf("END"); break;
            case EVENT_RESET: printf("RESET"); break;
            case EVENT_INVALID: printf("IN