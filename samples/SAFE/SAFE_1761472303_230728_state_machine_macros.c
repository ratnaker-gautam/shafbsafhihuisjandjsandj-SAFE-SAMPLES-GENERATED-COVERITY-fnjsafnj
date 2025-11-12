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

void state_machine_step(StateMachine* sm, int event, char data_char) {
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
            } else {
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
}

void process_data(StateMachine* sm) {
    if (sm->state != STATE_PROCESSING) return;
    
    printf("Processing data: ");
    for (size_t i = 0; i < sm->data_len; i++) {
        printf("%c", sm->data[i]);
    }
    printf("\n");
    
    for (size_t i = 0; i < sm->data_len; i++) {
        if (islower(sm->data[i])) {
            sm->data[i] = toupper(sm->data[i]);
        } else if (isupper(sm->data[i])) {
            sm->data[i] = tolower(sm->data[i]);
        }
    }
    
    printf("Processed data: ");
    for (size_t i = 0; i < sm->data_len; i++) {
        printf("%c", sm->data[i]);
    }
    printf("\n");
    
    sm->state = STATE_DONE;
}

int main() {
    char input[MAX_INPUT_LEN];
    StateMachine sm = {STATE_IDLE, {0}, 0};
    
    printf("Enter sequence (S=Start, Dx=Data, E=End, R=Reset): ");
    if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
        return 1;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    size_t pos = 0;
    int event;
    
    while ((event = get_next_event(input, &pos)) != EVENT_END) {
        char data_char = '\0';
        if (event == EVENT_DATA && pos < strlen(input)) {
            data_char = input[pos];
            pos++;
        }
        
        state_machine_step(&sm, event, data_char);
        
        if (sm.state == STATE_PROCESSING) {
            process_data(&sm);
        }
        
        if (sm.state == STATE_ERROR) {
            printf("Error state reached\n");
            break;
        }
    }
    
    if (sm.state == STATE_DONE) {
        printf("State machine completed successfully\n");
    } else if (sm.state != STATE_ERROR) {
        state_machine_step(&sm, EVENT_END, '\0');
        if (sm.state == STATE_PROCESSING) {
            process_data(&sm);
        }
    }
    
    return 0;
}