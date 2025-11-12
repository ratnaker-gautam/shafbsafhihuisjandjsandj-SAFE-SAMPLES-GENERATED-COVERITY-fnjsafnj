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

static int get_next_event(const char* input, size_t* pos) {
    if (input == NULL || pos == NULL) return EVENT_INVALID;
    
    size_t len = strlen(input);
    if (*pos >= len) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'D') return EVENT_DATA;
    if (c == 'E') return EVENT_END;
    if (c == 'R') return EVENT_RESET;
    
    return EVENT_INVALID;
}

static void state_machine_init(StateMachine* sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data, 0, MAX_DATA_LEN);
}

static int state_machine_step(StateMachine* sm, int event, char data_char) {
    if (sm == NULL) return STATE_ERROR;
    
    int next_state = sm->state;
    
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                next_state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data, 0, MAX_DATA_LEN);
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1 && isalnum(data_char)) {
                    sm->data[sm->data_len++] = data_char;
                    next_state = STATE_READING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    next_state = STATE_PROCESSING;
                } else {
                    next_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                next_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                sm->data_len = 0;
            } else if (event == EVENT_START) {
                next_state = STATE_READING;
                sm->data_len = 0;
            } else {
                next_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET || event == EVENT_START) {
                next_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                next_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    sm->state = next_state;
    return next_state;
}

static void print_state_info(int state, const char* data, size_t data_len) {
    const char* state_names[] = {"IDLE", "READING", "PROCESSING", "DONE", "ERROR"};
    
    if (state >= 0 && state <= 4) {
        printf("State: %s", state_names[state]);
        if (data_len > 0 && state != STATE_IDLE && state != STATE_ERROR && data != NULL) {
            printf(" | Data: ");
            for (size_t i = 0; i < data_len && i < MAX_DATA_LEN; i++) {
                putchar(data[i]);
            }
        }
        printf("\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter sequence (S=Start, D=Data, E=End, R=Reset, char after D is data): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    state_machine_init(&sm);
    
    printf("Initial ");
    print_state_info(sm.state, sm.data, sm.data_len);
    
    size_t pos = 0;
    size_t input_len = strlen(input);
    while (pos < input_len) {
        int event = get_next_event(input, &pos);
        char data_char = '\0';
        
        if (event == EVENT_DATA && pos < input_len) {
            data_char = input