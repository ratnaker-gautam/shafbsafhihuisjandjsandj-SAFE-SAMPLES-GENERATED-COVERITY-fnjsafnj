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

void process_data(const char* data) {
    printf("Processing: ");
    for (size_t i = 0; i < strlen(data); i++) {
        printf("%c", toupper(data[i]));
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
    while (pos < strlen(input)) {
        int event = get_next_event(input, &pos);
        char data_char = '\0';
        
        if (event == EVENT_DATA && pos < strlen(input)) {
            data_char = input[pos];
            pos++;
        }
        
        int new_state = state_machine_step(&sm, event, data_char);
        
        if (new_state == STATE_PROCESSING) {
            process_data(sm.data);
            sm.state = STATE_DONE;
        } else if (new_state == STATE_ERROR) {
            printf("Error: Invalid sequence\n");
            break;
        }
        
        if (new_state == STATE_DONE) {
            printf("Sequence completed successfully\n");
            break;
        }
    }
    
    if (sm.state == STATE_READING) {
        printf("Error: Incomplete sequence\n");
    }
    
    return 0;
}