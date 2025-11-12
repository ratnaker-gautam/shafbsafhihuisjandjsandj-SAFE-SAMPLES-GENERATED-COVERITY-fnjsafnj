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

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

static int transition_state(StateMachine* sm, int new_state) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (!CHECK_BOUNDS(new_state, 5)) return TRANSITION_ERROR;
    
    int valid_transition = 0;
    switch (sm->current_state) {
        case STATE_IDLE:
            valid_transition = (new_state == STATE_READING);
            break;
        case STATE_READING:
            valid_transition = (new_state == STATE_PROCESSING || new_state == STATE_ERROR);
            break;
        case STATE_PROCESSING:
            valid_transition = (new_state == STATE_WRITING || new_state == STATE_ERROR);
            break;
        case STATE_WRITING:
            valid_transition = (new_state == STATE_IDLE || new_state == STATE_ERROR);
            break;
        case STATE_ERROR:
            valid_transition = (new_state == STATE_IDLE);
            break;
        default:
            return TRANSITION_ERROR;
    }
    
    if (valid_transition) {
        sm->current_state = new_state;
        return TRANSITION_OK;
    }
    return TRANSITION_ERROR;
}

static int read_input(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (sm->current_state != STATE_READING) return TRANSITION_ERROR;
    
    char input[MAX_INPUT_LEN];
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return TRANSITION_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return TRANSITION_ERROR;
    }
    
    if (len >= MAX_BUFFER) {
        return TRANSITION_ERROR;
    }
    
    memcpy(sm->buffer, input, len + 1);
    sm->buffer_len = len;
    
    return TRANSITION_OK;
}

static int process_data(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (sm->current_state != STATE_PROCESSING) return TRANSITION_ERROR;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return TRANSITION_OK;
}

static int write_output(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (sm->current_state != STATE_WRITING) return TRANSITION_ERROR;
    
    printf("Processed output: %s\n", sm->buffer);
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    
    return TRANSITION_OK;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer[0] = '\0';
    sm.buffer_len = 0;
    
    int running = 1;
    char command[10];
    
    while (running) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nCommands: read, process, write, reset, quit\n> ");
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            transition_state(&sm, STATE_ERROR);
            continue;
        }
        
        command[strcspn(command, "\n")] = '\0';
        
        if (strcmp(command, "quit") == 0) {
            running = 0;
        } else if (strcmp(command, "read") == 0) {
            if (transition_state(&sm, STATE_READING) == TRANSITION_OK) {
                if (read_input(&sm) != TRANSITION_OK) {
                    transition_state(&sm, STATE_ERROR);
                } else {
                    transition_state(&sm, STATE_PROCESSING);
                }
            }
        } else if (strcmp(command