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
    if (*pos >= strlen(input)) return EVENT_END;
    
    char c = input[*pos];
    (*pos)++;
    
    if (c == 'S') return EVENT_START;
    if (c == 'D') return EVENT_DATA;
    if (c == 'E') return EVENT_END;
    if (c == 'R') return EVENT_RESET;
    
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
                if (sm->data_len < MAX_DATA_LEN - 1 && isalnum(data_char)) {
                    sm->data[sm->data_len++] = data_char;
                }
            } else if (event == EVENT_END) {
                sm->state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else if (event == EVENT_INVALID) {
                sm->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
                sm->data_len = 0;
            } else if (event == EVENT_START) {
                sm->state = STATE_READING;
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

void print_state_info(int state, const char* data, size_t data_len) {
    const char* state_names[] = {"IDLE", "READING", "PROCESSING", "DONE", "ERROR"};
    printf("State: %s", state_names[state]);
    
    if (data_len > 0) {
        printf(" | Data: ");
        for (size_t i = 0; i < data_len; i++) {
            putchar(data[i]);
        }
    }
    printf("\n");
}

int main() {
    char input[MAX_INPUT_LEN];
    StateMachine sm;
    
    printf("Enter sequence (S=Start, Dx=Data, E=End, R=Reset): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    state_machine_init(&sm);
    size_t pos = 0;
    
    printf("Initial ");
    print_state_info(sm.state, sm.data, sm.data_len);
    
    while (pos < strlen(input)) {
        int event = get_next_event(input, &pos);
        char data_char = (event == EVENT_DATA && pos < strlen(input)) ? input[pos++] : '\0';
        
        int new_state = state_machine_step(&sm, event, data_char);
        
        printf("Event: ");
        switch (event) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA: printf("DATA('%c')", data_char); break;
            case EVENT_END: printf("END"); break;
            case EVENT_RESET: printf("RESET"); break;
            case EVENT_INVALID: printf("INVALID"); break;
        }
        printf(" -> ");
        print_state_info(new_state, sm.data, sm.data_len);
        
        if (sm.state == STATE_ERROR) {
            printf("Error state reached\n");
            break;
        }
    }
    
    if (sm.state == STATE_DONE) {
        printf("Processing complete. Final data: ");
        for (size_t i = 0; i < sm.data_len; i++) {
            putchar(sm.data[i]);
        }
        printf("\n");
    }
    
    return 0;
}