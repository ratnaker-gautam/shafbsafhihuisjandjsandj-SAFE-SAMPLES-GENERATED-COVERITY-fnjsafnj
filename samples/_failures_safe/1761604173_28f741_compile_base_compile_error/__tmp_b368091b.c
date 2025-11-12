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
    size_t data_length;
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
    sm->data_length = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
}

static void process_state_machine(state_machine_t *sm, int event, char data_char) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_length = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("Transition: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_length < MAX_DATA_LEN - 1 && isalnum(data_char)) {
                    sm->data_buffer[sm->data_length++] = data_char;
                    printf("Added data: %c\n", data_char);
                }
            } else if (event == EVENT_END) {
                if (sm->data_length > 0) {
                    sm->current_state = STATE_PROCESSING;
                    printf("Transition: READING -> PROCESSING\n");
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("Transition: READING -> ERROR (no data)\n");
                }
            } else if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_length = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("Transition: READING -> IDLE (reset)\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_length = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("Transition: PROCESSING -> IDLE (reset)\n");
            } else {
                if (sm->data_length < MAX_DATA_LEN) {
                    sm->data_buffer[sm->data_length] = '\0';
                } else {
                    sm->data_buffer[MAX_DATA_LEN - 1] = '\0';
                }
                printf("Processing data: %s\n", sm->data_buffer);
                sm->current_state = STATE_DONE;
                printf("Transition: PROCESSING -> DONE\n");
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_length = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("Transition: DONE -> IDLE (reset)\n");
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_length = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("Transition: ERROR -> IDLE (reset)\n");
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
    printf("For DATA command, provide alphanumeric character after D\n");
    printf("Enter commands (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        int event = get_next_event(input_buffer[i]);
        char data_char = '\0';
        
        if (event == EVENT_DATA && i + 1 < input_len) {
            data_char = input_buffer[i + 1];
            i++;
        }
        
        process_state_machine(&sm, event, data_char);
        
        if (sm.current_state == STATE_DONE || sm.current_state == STATE_ERROR)