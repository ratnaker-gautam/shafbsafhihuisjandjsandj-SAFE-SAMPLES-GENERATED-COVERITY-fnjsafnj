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

const char* state_names[] = {"IDLE", "READING", "PROCESSING", "DONE", "ERROR"};
const char* event_names[] = {"START", "DATA", "END", "RESET", "INVALID"};

void state_machine_init(StateMachine* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, sizeof(sm->data));
}

int state_machine_handle_event(StateMachine* sm, int event, const char* input) {
    int new_state = sm->state;
    
    switch(sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                new_state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (input != NULL && sm->data_len < MAX_DATA_LEN - 1) {
                    size_t input_len = strlen(input);
                    if (input_len > 0 && input_len <= MAX_DATA_LEN - sm->data_len - 1) {
                        strncat(sm->data, input, MAX_DATA_LEN - sm->data_len - 1);
                        sm->data_len += input_len;
                    }
                }
                new_state = STATE_READING;
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    new_state = STATE_PROCESSING;
                } else {
                    new_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            } else {
                for (size_t i = 0; i < sm->data_len; i++) {
                    if (isalpha(sm->data[i])) {
                        sm->data[i] = toupper(sm->data[i]);
                    }
                }
                new_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                new_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, sizeof(sm->data));
            }
            break;
            
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    sm->state = new_state;
    return new_state;
}

int get_event_from_input(const char* input) {
    if (input == NULL) return EVENT_INVALID;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_INVALID;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, end, reset, quit\n");
    printf("Current state: %s\n", state_names[sm.state]);
    
    while (1) {
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        char* space = strchr(input, ' ');
        char* data_part = NULL;
        int event;
        
        if (space != NULL) {
            *space = 0;
            data_part = space + 1;
            if (strlen(data_part) == 0) {
                data_part = NULL;
            }
        }
        
        event = get_event_from_input(input);
        
        if (event == EVENT_INVALID) {
            printf("Invalid command\n");
            continue;
        }
        
        int new_state = state_machine_handle_event(&sm,