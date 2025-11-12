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
                printf("Transition: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (input_data != NULL) {
                    size_t data_len = strlen(input_data);
                    if (data_len > 0 && data_len < MAX_DATA_LEN && 
                        sm->data_len + data_len < MAX_DATA_LEN) {
                        strncpy(sm->data + sm->data_len, input_data, data_len);
                        sm->data_len += data_len;
                        sm->state = STATE_PROCESSING;
                        printf("Transition: READING -> PROCESSING (Data: %s)\n", input_data);
                    } else {
                        sm->state = STATE_ERROR;
                        printf("Transition: READING -> ERROR (Data too long)\n");
                    }
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
                printf("Transition: READING -> DONE\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DATA) {
                sm->state = STATE_READING;
                printf("Transition: PROCESSING -> READING\n");
            } else if (event == EVENT_END) {
                sm->state = STATE_DONE;
                printf("Transition: PROCESSING -> DONE\n");
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("Transition: DONE -> IDLE (Reset)\n");
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("Transition: ERROR -> IDLE (Reset)\n");
            }
            break;
    }
    
    return sm->state;
}

void print_state_info(int state) {
    switch (state) {
        case STATE_IDLE: printf("Current state: IDLE\n"); break;
        case STATE_READING: printf("Current state: READING\n"); break;
        case STATE_PROCESSING: printf("Current state: PROCESSING\n"); break;
        case STATE_DONE: printf("Current state: DONE\n"); break;
        case STATE_ERROR: printf("Current state: ERROR\n"); break;
        default: printf("Current state: UNKNOWN\n"); break;
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    char data_input[MAX_INPUT_LEN];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data <value>, end, reset, quit\n");
    
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
        
        char command[20];
        char data_value[MAX_DATA_LEN] = {0};
        
        if (sscanf(input, "%19s %49[^\n]", command, data_value) >= 1) {
            int event = get_event(command);
            
            if (event == EVENT_DATA && strlen(data_value) == 0) {
                printf("Error: 'data' command requires a value\n");
                continue;
            }
            
            state_machine_step(&sm, event, data_value);
            print_state_info(sm.state);
            
            if (sm.state == STATE_DONE) {
                printf("Processing complete. Final data: %s\n", sm.data);
            }
        } else {
            printf("Error: Invalid command format\n");