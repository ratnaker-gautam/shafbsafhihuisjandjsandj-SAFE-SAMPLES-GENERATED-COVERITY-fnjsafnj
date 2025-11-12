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
    int current_state;
    char data_buffer[MAX_DATA_LEN];
    size_t data_len;
} state_machine_t;

void state_machine_init(state_machine_t *sm) {
    sm->current_state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
}

int get_event(const char *input) {
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return EVENT_INVALID;
}

void process_state_machine(state_machine_t *sm, int event, const char *user_input) {
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                printf("State: READING - Ready to receive data\n");
            } else if (event == EVENT_RESET) {
                printf("State: IDLE - Already idle\n");
            } else {
                sm->current_state = STATE_ERROR;
                printf("State: ERROR - Invalid event for IDLE state\n");
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (user_input != NULL && strlen(user_input) > 0) {
                    size_t input_len = strlen(user_input);
                    if (sm->data_len + input_len < MAX_DATA_LEN) {
                        strncpy(sm->data_buffer + sm->data_len, user_input, input_len);
                        sm->data_len += input_len;
                        sm->current_state = STATE_PROCESSING;
                        printf("State: PROCESSING - Data received: %s\n", user_input);
                    } else {
                        sm->current_state = STATE_ERROR;
                        printf("State: ERROR - Data buffer overflow\n");
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("State: ERROR - No data provided\n");
                }
            } else if (event == EVENT_END) {
                sm->current_state = STATE_DONE;
                printf("State: DONE - No data processed\n");
            } else if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State: IDLE - Machine reset\n");
            } else {
                sm->current_state = STATE_ERROR;
                printf("State: ERROR - Invalid event for READING state\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                if (user_input != NULL && strlen(user_input) > 0) {
                    size_t input_len = strlen(user_input);
                    if (sm->data_len + input_len < MAX_DATA_LEN) {
                        strncpy(sm->data_buffer + sm->data_len, user_input, input_len);
                        sm->data_len += input_len;
                        printf("State: PROCESSING - Additional data: %s\n", user_input);
                    } else {
                        sm->current_state = STATE_ERROR;
                        printf("State: ERROR - Data buffer overflow\n");
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("State: ERROR - No data provided\n");
                }
            } else if (event == EVENT_END) {
                sm->current_state = STATE_DONE;
                printf("State: DONE - Final data: %s\n", sm->data_buffer);
            } else if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State: IDLE - Machine reset\n");
            } else {
                sm->current_state = STATE_ERROR;
                printf("State: ERROR - Invalid event for PROCESSING state\n");
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State: IDLE - Machine reset\n");
            } else {
                printf("State: DONE - Use 'reset' to restart\n");
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State: IDLE - Machine reset\n");
            } else {
                printf("State: ERROR - Use 'reset' to recover\n");
            }
            break;
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data <text>, end, reset, quit\n");
    
    while (1) {
        printf("\nEnter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Input error\n");
            continue;
        }
        
        size_t len = strlen(input_buffer);
        if (