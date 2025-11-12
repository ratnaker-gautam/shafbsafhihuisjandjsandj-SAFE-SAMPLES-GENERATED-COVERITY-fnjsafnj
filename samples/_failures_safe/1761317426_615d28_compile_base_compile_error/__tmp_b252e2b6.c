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

int state_machine_handle_event(StateMachine *sm, int event, const char *input) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                return 1;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (input != NULL && sm->data_len < MAX_DATA_LEN - 1) {
                    size_t input_len = strlen(input);
                    if (input_len > 0 && input_len <= MAX_DATA_LEN - sm->data_len - 1) {
                        strncpy(sm->data + sm->data_len, input, input_len);
                        sm->data_len += input_len;
                        sm->data[sm->data_len] = '\0';
                        return 1;
                    }
                }
                sm->state = STATE_ERROR;
                return 0;
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->state = STATE_PROCESSING;
                    return 1;
                } else {
                    sm->state = STATE_ERROR;
                    return 0;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
                return 1;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
                return 1;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
                return 1;
            }
            break;
    }
    return 0;
}

void process_data(StateMachine *sm) {
    if (sm->state != STATE_PROCESSING) return;
    
    for (size_t i = 0; i < sm->data_len; i++) {
        sm->data[i] = toupper(sm->data[i]);
    }
    sm->state = STATE_DONE;
}

int get_event_from_input(const char *input) {
    if (input == NULL) return EVENT_INVALID;
    
    if (strcmp(input, "START") == 0) return EVENT_START;
    if (strcmp(input, "DATA") == 0) return EVENT_DATA;
    if (strcmp(input, "END") == 0) return EVENT_END;
    if (strcmp(input, "RESET") == 0) return EVENT_RESET;
    
    return EVENT_INVALID;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Controller\n");
    printf("Commands: START, DATA <text>, END, RESET, QUIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "QUIT") == 0) {
            break;
        }
        
        int event;
        char *data_ptr = NULL;
        
        if (strncmp(input, "DATA ", 5) == 0) {
            event = EVENT_DATA;
            data_ptr = input + 5;
            if (strlen(data_ptr) == 0) {
                printf("Error: DATA command requires text\n");
                continue;
            }
        } else {
            event = get_event_from_input(input);
            data_ptr = NULL;
        }
        
        if (event == EVENT_INVALID) {
            printf("Error: Invalid command\n");
            continue;
        }
        
        int result = state_machine_handle_event(&sm, event, data_ptr);
        
        if (result