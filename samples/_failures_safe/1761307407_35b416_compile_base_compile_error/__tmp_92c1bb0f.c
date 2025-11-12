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
    if (!sm) return;
    sm->current_state = STATE_IDLE;
    sm->data_len = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
}

static int state_machine_step(state_machine_t *sm, int event, char data_char) {
    if (!sm) return STATE_ERROR;
    
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
                    sm->data_buffer[sm->data_len++] = data_char;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            } else if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_len = 0;
            } else {
                for (size_t i = 0; i < sm->data_len; i++) {
                    if (isalpha((unsigned char)sm->data_buffer[i])) {
                        sm->data_buffer[i] = toupper((unsigned char)sm->data_buffer[i]);
                    }
                }
                sm->current_state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET || event == EVENT_START) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
            }
            break;
            
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
    
    return sm->current_state;
}

static const char* state_to_string(int state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_DONE: return "DONE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    state_machine_t sm;
    char input_line[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, Dx=Data(x), E=End, R=Reset\n");
    printf("Enter commands (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    printf("\nInitial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; i < input_len; i++) {
        int event = get_next_event(input_line[i]);
        char data_char = '\0';
        
        if (event == EVENT_DATA && i + 1 < input_len) {
            data_char = input_line[i + 1];
            i++;
        }
        
        int new_state = state_machine_step(&sm, event, data_char);
        
        printf("Event: ");
        switch (event) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA: printf("DATA('%c')", data_char); break;
            case EVENT_END: printf