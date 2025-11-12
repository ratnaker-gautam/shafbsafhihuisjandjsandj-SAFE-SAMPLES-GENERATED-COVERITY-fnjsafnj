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

int get_next_event(const char* input, size_t* pos) {
    if (input == NULL || pos == NULL) return EVENT_INVALID;
    size_t input_len = strlen(input);
    if (*pos >= input_len) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'D') return EVENT_DATA;
    if (c == 'E') return EVENT_END;
    if (c == 'R') return EVENT_RESET;
    
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
                memset(sm->data, 0, MAX_DATA_LEN);
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1 && isalnum((unsigned char)data_char)) {
                    sm->data[sm->data_len++] = data_char;
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
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                sm->state = STATE_DONE;
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

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->state != STATE_PROCESSING) return;
    
    printf("Processing data: ");
    for (size_t i = 0; i < sm->data_len; i++) {
        printf("%c", sm->data[i]);
    }
    printf("\n");
    
    for (size_t i = 0; i < sm->data_len; i++) {
        if (islower((unsigned char)sm->data[i])) {
            sm->data[i] = (char)toupper((unsigned char)sm->data[i]);
        } else if (isupper((unsigned char)sm->data[i])) {
            sm->data[i] = (char)tolower((unsigned char)sm->data[i]);
        }
    }
    
    printf("Processed data: ");
    for (size_t i = 0; i < sm->data_len; i++) {
        printf("%c", sm->data[i]);
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter state machine sequence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_machine_init(&sm);
    
    size_t pos = 0;
    int event;
    
    while ((event = get_next_event(input, &pos)) != EVENT_END) {
        char data_char = '\0';
        if (event == EVENT_DATA && pos < strlen(input)) {
            data_char = input[pos];
        }
        
        int old_state = sm.state;
        int new_state = state_machine_step(&sm, event, data_char);
        
        if (event == EVENT_DATA && data_char != '\0') {
            pos++;
        }
        
        if (new_state == STATE_PROCESSING && old_state != STATE