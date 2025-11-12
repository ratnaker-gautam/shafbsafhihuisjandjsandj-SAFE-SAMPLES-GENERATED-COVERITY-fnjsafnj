//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
    size_t data_length;
    int error_count;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    sm->data_length = 0;
    sm->error_count = 0;
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

void process_state_machine(StateMachine *sm, int event, const char *input_data) {
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                printf("Transition: IDLE -> READING\n");
                sm->current_state = STATE_READING;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                sm->data_length = 0;
            } else if (event == EVENT_RESET) {
                printf("Already in IDLE state\n");
            } else {
                printf("Invalid event for IDLE state\n");
                sm->error_count++;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (input_data != NULL) {
                    size_t data_len = strlen(input_data);
                    if (data_len > 0 && data_len < MAX_DATA_LEN && 
                        sm->data_length + data_len < MAX_DATA_LEN) {
                        strncpy(sm->data_buffer + sm->data_length, input_data, 
                               MAX_DATA_LEN - sm->data_length - 1);
                        sm->data_length += data_len;
                        sm->data_buffer[sm->data_length] = '\0';
                        printf("Data collected: %s\n", sm->data_buffer);
                    } else {
                        printf("Data too long or invalid\n");
                        sm->error_count++;
                    }
                }
            } else if (event == EVENT_END) {
                if (sm->data_length > 0) {
                    printf("Transition: READING -> PROCESSING\n");
                    sm->current_state = STATE_PROCESSING;
                } else {
                    printf("No data to process\n");
                    sm->error_count++;
                }
            } else if (event == EVENT_RESET) {
                printf("Transition: READING -> IDLE\n");
                sm->current_state = STATE_IDLE;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                sm->data_length = 0;
            } else {
                printf("Invalid event for READING state\n");
                sm->error_count++;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_END) {
                printf("Processing complete: %s\n", sm->data_buffer);
                printf("Transition: PROCESSING -> DONE\n");
                sm->current_state = STATE_DONE;
            } else if (event == EVENT_RESET) {
                printf("Transition: PROCESSING -> IDLE\n");
                sm->current_state = STATE_IDLE;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                sm->data_length = 0;
            } else {
                printf("Invalid event for PROCESSING state\n");
                sm->error_count++;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                printf("Transition: DONE -> IDLE\n");
                sm->current_state = STATE_IDLE;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                sm->data_length = 0;
            } else {
                printf("Invalid event for DONE state\n");
                sm->error_count++;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                printf("Transition: ERROR -> IDLE\n");
                sm->current_state = STATE_IDLE;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                sm->data_length = 0;
                sm->error_count = 0;
            } else {
                printf("Invalid event for ERROR state\n");
                sm->error_count++;
            }
            break;
            
        default:
            printf("Unknown state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    char data_input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf