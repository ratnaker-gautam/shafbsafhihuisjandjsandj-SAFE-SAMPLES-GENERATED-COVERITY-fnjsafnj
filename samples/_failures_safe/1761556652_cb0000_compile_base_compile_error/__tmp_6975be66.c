//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 256

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_PROCESS 2
#define EVENT_WRITE 3
#define EVENT_ERROR 4
#define EVENT_RESET 5

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} state_machine_t;

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static int handle_event(state_machine_t* sm, int event, const char* data, size_t data_len) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->buffer_len = 0;
                return TRANSITION_OK;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (data == NULL || data_len == 0 || data_len > MAX_INPUT_LEN) {
                    return TRANSITION_ERROR;
                }
                if (sm->buffer_len + data_len >= MAX_BUFFER) {
                    return TRANSITION_ERROR;
                }
                memcpy(sm->buffer + sm->buffer_len, data, data_len);
                sm->buffer_len += data_len;
                return TRANSITION_OK;
            } else if (event == EVENT_PROCESS && sm->buffer_len > 0) {
                sm->current_state = STATE_PROCESSING;
                return TRANSITION_OK;
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return TRANSITION_OK;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_WRITE) {
                sm->current_state = STATE_WRITING;
                return TRANSITION_OK;
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return TRANSITION_OK;
            }
            break;
            
        case STATE_WRITING:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->buffer_len = 0;
                return TRANSITION_OK;
            } else if (event == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                return TRANSITION_OK;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->buffer_len = 0;
                return TRANSITION_OK;
            }
            break;
    }
    
    return TRANSITION_ERROR;
}

static void process_buffer(state_machine_t* sm) {
    if (sm == NULL || sm->buffer_len == 0) return;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    
    char input[MAX_INPUT_LEN + 1];
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("start, data <text>, process, write, error, reset, quit\n");
    
    while (running) {
        printf("State: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        } else if (strcmp(input, "start") == 0) {
            if (handle_event(&sm, EVENT_START, NULL, 0) == TRANSITION_ERROR) {
                printf("Invalid transition from current state\n");
            }
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* text = input + 5;
            size_t len = strlen(text);
            if (validate_input(text, len)) {
                if (handle_event(&sm, EVENT_DATA, text, len) == TRANSITION_ERROR) {
                    printf("Invalid data or transition\n");
                }
            } else {
                printf("Invalid