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
    int current_state;
    char data_buffer[MAX_DATA_LEN];
    size_t data_len;
} state_machine_t;

static int get_next_event(const char input) {
    if (input == 'S') return EVENT_START;
    if (input == 'D') return EVENT_DATA;
    if (input == 'E') return EVENT_END;
    if (input == 'R') return EVENT_RESET;
    return EVENT_INVALID;
}

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
}

static void process_state_machine(state_machine_t *sm, int event, char data) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data_buffer[sm->data_len++] = data;
                }
            } else if (event == EVENT_END) {
                sm->current_state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            } else {
                printf("Processing data: ");
                for (size_t i = 0; i < sm->data_len; i++) {
                    if (isalnum((unsigned char)sm->data_buffer[i])) {
                        putchar(sm->data_buffer[i]);
                    }
                }
                printf("\n");
                sm->current_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
}

int main(void) {
    state_machine_t sm;
    char input_buffer[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("Enter commands (S=Start, D=Data, E=End, R=Reset): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    for (size_t i = 0; i < input_len; i++) {
        int event = get_next_event(input_buffer[i]);
        char data_char = (event == EVENT_DATA && i + 1 < input_len) ? input_buffer[++i] : '\0';
        
        process_state_machine(&sm, event, data_char);
        
        if (sm.current_state == STATE_ERROR) {
            printf("State machine entered error state\n");
            break;
        }
    }
    
    return EXIT_SUCCESS;
}