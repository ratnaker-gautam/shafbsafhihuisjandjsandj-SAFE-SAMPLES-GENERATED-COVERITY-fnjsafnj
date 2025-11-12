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
#define MAX_DATA_COUNT 10

typedef struct {
    int state;
    int data_count;
    char input_buffer[MAX_INPUT_LEN + 1];
    int buffer_pos;
} state_machine_t;

static int get_next_event(const char* input, int* pos) {
    if (*pos < 0 || input == NULL) return EVENT_INVALID;
    
    int current_pos = *pos;
    if (input[current_pos] == '\0') return EVENT_END;
    
    if (current_pos == 0 && input[0] != '\0') return EVENT_START;
    
    if (input[current_pos] != '\0') return EVENT_DATA;
    
    return EVENT_INVALID;
}

static void state_machine_init(state_machine_t* sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

static int state_machine_step(state_machine_t* sm, int event, char data) {
    if (sm == NULL || event < EVENT_START || event > EVENT_INVALID) {
        return STATE_ERROR;
    }
    
    switch (sm->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->state = STATE_READING;
                sm->buffer_pos = 0;
                sm->data_count = 0;
                memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (sm->buffer_pos < MAX_INPUT_LEN && isprint(data)) {
                    sm->input_buffer[sm->buffer_pos] = data;
                    sm->buffer_pos++;
                    sm->input_buffer[sm->buffer_pos] = '\0';
                }
            } else if (event == EVENT_END) {
                if (sm->buffer_pos > 0) {
                    sm->state = STATE_PROCESSING;
                } else {
                    sm->state = STATE_ERROR;
                }
            } else if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
            } else {
                sm->data_count++;
                if (sm->data_count >= MAX_DATA_COUNT) {
                    sm->state = STATE_DONE;
                } else {
                    sm->state = STATE_READING;
                    sm->buffer_pos = 0;
                }
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->state = STATE_IDLE;
            }
            break;
            
        default:
            sm->state = STATE_ERROR;
            break;
    }
    
    return sm->state;
}

static void print_state_info(int state) {
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("State: %s\n", state_names[state]);
    } else {
        printf("State: UNKNOWN\n");
    }
}

int main(void) {
    state_machine_t sm;
    char user_input[MAX_INPUT_LEN + 1];
    int input_len;
    int i;
    int event;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo - Enter text sequences (empty line to process, 'reset' to reset, 'quit' to exit)\n");
    
    while (1) {
        print_state_info(sm.state);
        
        if (sm.state == STATE_DONE) {
            printf("Processing complete. Data count: %d\n", sm.data_count);
            printf("Final buffer: %s\n", sm.input_buffer);
            printf("Enter 'reset' to continue or 'quit' to exit: ");
        } else if (sm.state == STATE_ERROR) {
            printf("Error occurred. Enter 'reset' to continue: ");
        } else {
            printf("Enter text: ");
        }
        
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            break;
        }
        
        input_len = (int)strlen(user_input);
        if (input_len > 0 && user_input[input_len - 1] == '\n') {
            user_input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (strcmp(user_input, "quit") == 0) {
            break;
        }
        
        if (strcmp(user_input, "reset") == 0) {
            state_machine_step(&sm, EVENT_RESET, 0);
            continue;
        }
        
        if (sm.state == STATE_IDLE