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
    if (*pos >= strlen(input)) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'R') return EVENT_RESET;
    if (isalnum(c)) return EVENT_DATA;
    return EVENT_INVALID;
}

void state_machine_init(StateMachine* sm) {
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, MAX_DATA_LEN);
}

int state_machine_step(StateMachine* sm, int event, char data_char) {
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data[sm->data_len++] = data_char;
                    sm->data[sm->data_len] = '\0';
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->state = STATE_PROCESSING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            } else {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            } else if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            } else {
                sm->state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET || event == EVENT_START) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            }
            break;
            
        default:
            sm->state = STATE_ERROR;
            break;
    }
    
    return sm->state;
}

void process_data(const char* data) {
    printf("Processing: ");
    for (size_t i = 0; i < strlen(data); i++) {
        if (isalnum(data[i])) {
            printf("%c", data[i]);
        }
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter sequence (S=start, R=reset, alphanum=data, end=EOF): ");
    
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return 1;
    }
    
    state_machine_init(&sm);
    
    size_t pos = 0;
    int event;
    
    while ((event = get_event(input, &pos)) != EVENT_END && sm.state != STATE_ERROR) {
        char data_char = (pos > 0 && event == EVENT_DATA) ? input[pos-1] : '\0';
        int new_state = state_machine_step(&sm, event, data_char);
        
        if (new_state == STATE_PROCESSING) {
            process_data(sm.data);
            sm.state = STATE_DONE;
        } else if (new_state == STATE_ERROR) {
            printf("Error: Invalid state transition\n");
        }
    }
    
    if (sm.state == STATE_READING) {
        printf("Error: Unexpected end of input\n");
    } else if (sm.state == STATE_DONE) {
        printf("Completed successfully\n");
    }
    
    return 0;
}