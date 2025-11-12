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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int get_event(const char* input, size_t len) {
    if (len == 0) return EVENT_INVALID;
    
    if (strncmp(input, "START", 5) == 0 && (len == 5 || isspace(input[5]))) return EVENT_START;
    if (strncmp(input, "DATA", 4) == 0 && (len == 4 || isspace(input[4]))) return EVENT_DATA;
    if (strncmp(input, "END", 3) == 0 && (len == 3 || isspace(input[3]))) return EVENT_END;
    if (strncmp(input, "RESET", 5) == 0 && (len == 5 || isspace(input[5]))) return EVENT_RESET;
    
    return EVENT_INVALID;
}

void state_machine_init(StateMachine* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, MAX_DATA_LEN);
}

int state_machine_handle_event(StateMachine* sm, int event, const char* input_data, size_t data_len) {
    if (sm == NULL) return 0;
    
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
                return 1;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (input_data != NULL && data_len > 0) {
                    size_t copy_len = data_len;
                    if (copy_len > MAX_DATA_LEN - sm->data_len - 1) {
                        copy_len = MAX_DATA_LEN - sm->data_len - 1;
                    }
                    if (copy_len > 0) {
                        memcpy(sm->data + sm->data_len, input_data, copy_len);
                        sm->data_len += copy_len;
                        sm->data[sm->data_len] = '\0';
                    }
                }
                return 1;
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
                memset(sm->data, 0, MAX_DATA_LEN);
                return 1;
            } else if (event == EVENT_END) {
                printf("Processed data: %s\n", sm->data);
                sm->state = STATE_DONE;
                return 1;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
                return 1;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
                return 1;
            }
            break;
    }
    
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo\n");
    printf("Commands: START, DATA <text>, END, RESET\n");
    printf("Current state: %s\n", state_names[sm.state]);
    
    while (sm.state != STATE_DONE) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (!validate