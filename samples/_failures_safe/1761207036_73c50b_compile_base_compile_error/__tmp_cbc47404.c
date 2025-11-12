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

int validate_input(const char* input, size_t len) {
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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

int process_event(StateMachine* sm, int event, const char* input, size_t len) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                return 1;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                const char* data_start = input + 5;
                size_t data_len = len - 5;
                while (data_len > 0 && isspace(*data_start)) {
                    data_start++;
                    data_len--;
                }
                
                if (data_len > 0 && data_len <= MAX_DATA_LEN - sm->data_len - 1) {
                    memcpy(sm->data + sm->data_len, data_start, data_len);
                    sm->data_len += data_len;
                    sm->data[sm->data_len] = '\0';
                    return 1;
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->state = STATE_PROCESSING;
                    return 1;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_END) {
                printf("Processed data: %s\n", sm->data);
                sm->state = STATE_DONE;
                return 1;
            }
            break;
            
        case STATE_DONE:
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
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
    
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[--len] = '\0';
        }
        
        if (!validate_input(input, len)) {
            printf("Invalid input\n");
            continue;
        }
        
        if (strncmp(input, "QUIT", 4) == 0 && (len == 4 || isspace(input[4]))) {
            break;
        }
        
        int event = get_event(input, len);
        if (event == EVENT_INVALID) {
            printf("Unknown command\n");
            continue;
        }
        
        int result = process_event(&sm, event, input, len);
        if (!result) {
            printf("Invalid transition from %s with %s\n", 
                   state_names[sm.state], event_names[event]);
        } else {
            printf("State: %s", state_names[sm.state]);
            if (sm.state == STATE_READING && sm.data_len > 0) {
                printf(" (Data: %s)", sm.data);
            }
            printf("\n");
        }
    }