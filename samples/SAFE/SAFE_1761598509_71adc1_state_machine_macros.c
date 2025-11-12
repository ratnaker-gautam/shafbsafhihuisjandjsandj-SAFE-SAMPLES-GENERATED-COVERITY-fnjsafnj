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
    if (input == 'S' || input == 's') return EVENT_START;
    if (input == 'D' || input == 'd') return EVENT_DATA;
    if (input == 'E' || input == 'e') return EVENT_END;
    if (input == 'R' || input == 'r') return EVENT_RESET;
    return EVENT_INVALID;
}

static void state_machine_init(state_machine_t *sm) {
    if (!sm) return;
    sm->current_state = STATE_IDLE;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    sm->data_len = 0;
}

static void process_state_machine(state_machine_t *sm, int event, char data_char) {
    if (!sm) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                printf("State: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1) {
                    sm->data_buffer[sm->data_len++] = data_char;
                    printf("Added data: %c\n", data_char);
                } else {
                    printf("Data buffer full\n");
                }
            } else if (event == EVENT_END) {
                if (sm->data_len > 0) {
                    sm->current_state = STATE_PROCESSING;
                    printf("State: READING -> PROCESSING\n");
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("Error: No data to process\n");
                }
            } else if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State reset to IDLE\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State reset to IDLE\n");
            } else {
                sm->data_buffer[sm->data_len] = '\0';
                printf("Processing data: %s\n", sm->data_buffer);
                sm->current_state = STATE_DONE;
                printf("State: PROCESSING -> DONE\n");
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State reset to IDLE\n");
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
                printf("State reset to IDLE\n");
            }
            break;
    }
}

int main(void) {
    state_machine_t sm;
    char input_buffer[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, D=Data, E=End, R=Reset\n");
    printf("Enter commands (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input_buffer[i];
        int event = get_next_event(current_char);
        char data_char = 'X';
        
        if (event == EVENT_DATA && i + 1 < input_len) {
            data_char = input_buffer[i + 1];
            if (!isalnum((unsigned char)data_char)) {
                data_char = 'X';
            }
            i++;
        }
        
        process_state_machine(&sm, event, data_char);
        
        if (sm.current_state == STATE_ERROR) {
            printf("Machine entered error state\n");
            break;
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("Machine completed successfully\n");
    } else if (sm.current_state != STATE_ERROR) {
        printf("Machine did not reach completion\n");
    }
    
    return EXIT_SUCCESS;
}