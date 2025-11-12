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

void state_machine_init(StateMachine *sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, sizeof(sm->data));
}

int get_event(const char *input) {
    if (input == NULL) return EVENT_INVALID;
    
    size_t len = strlen(input);
    if (len == 0) return EVENT_INVALID;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_INVALID;
}

int state_machine_step(StateMachine *sm, int event, const char *input_data) {
    if (sm == NULL) return STATE_ERROR;
    
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
                if (input_data != NULL) {
                    size_t data_len = strlen(input_data);
                    if (data_len > 0 && sm->data_len + data_len < MAX_DATA_LEN) {
                        strncpy(sm->data + sm->data_len, input_data, data_len);
                        sm->data_len += data_len;
                        sm->state = STATE_PROCESSING;
                    } else {
                        sm->state = STATE_ERROR;
                    }
                }
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->state = STATE_DONE;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else {
                sm->state = STATE_ERROR;
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

void print_state(int state) {
    switch (state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_DONE: printf("DONE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    char data_input[MAX_INPUT_LEN];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data, end, reset, quit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        print_state(sm.state);
        if (sm.state == STATE_DONE && sm.data_len > 0) {
            printf(" (Data: %s)", sm.data);
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        int event = get_event(input);
        
        if (event == EVENT_DATA) {
            printf("Enter data: ");
            if (fgets(data_input, sizeof(data_input), stdin) == NULL) {
                break;
            }
            data_input[strcspn(data_input, "\n")] = 0;
        } else {
            data_input[0] = '\0';