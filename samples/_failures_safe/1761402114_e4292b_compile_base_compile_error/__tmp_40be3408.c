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

static void process_state_machine(state_machine_t *sm, int event, char data_char) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("State: IDLE -> READING\n");
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->data_len < MAX_DATA_LEN - 1 && isalnum(data_char)) {
                    sm->data_buffer[sm->data_len++] = data_char;
                    printf("Added data: %c\n", data_char);
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
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("State: READING -> IDLE (reset)\n");
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("State: PROCESSING -> IDLE (reset)\n");
            } else {
                printf("Processing data: %s\n", sm->data_buffer);
                sm->current_state = STATE_DONE;
                printf("State: PROCESSING -> DONE\n");
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("State: DONE -> IDLE (reset)\n");
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_len = 0;
                memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
                printf("State: ERROR -> IDLE (reset)\n");
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
    printf("For Data events, provide character after D (e.g., 'Da')\n");
    printf("Enter 'quit' to exit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char command = input_buffer[0];
        char data_char = '\0';
        
        if (strlen(input_buffer) > 1) {
            data_char = input_buffer[1];
        }
        
        int event = get_next_event(command);
        process_state_machine(&sm, event