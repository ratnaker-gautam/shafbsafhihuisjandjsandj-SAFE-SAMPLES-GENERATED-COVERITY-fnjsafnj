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
#define MAX_BUFFER_SIZE 50

#define TRANSITION(sm, new_state) do { (sm)->state = (new_state); } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

int state_machine_process(StateMachine *sm, const char *input, size_t input_len) {
    if (sm == NULL || !state_machine_validate_input(input, input_len)) {
        return -1;
    }

    switch (sm->state) {
        case STATE_IDLE:
            if (input_len > 0) {
                TRANSITION(sm, STATE_READING);
            }
            break;
            
        case STATE_READING:
            if (sm->buffer_len + input_len < MAX_BUFFER_SIZE) {
                memcpy(sm->buffer + sm->buffer_len, input, input_len);
                sm->buffer_len += input_len;
                if (sm->buffer_len >= MAX_BUFFER_SIZE / 2) {
                    TRANSITION(sm, STATE_PROCESSING);
                }
            } else {
                sm->error_code = 1;
                TRANSITION(sm, STATE_ERROR);
            }
            break;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            TRANSITION(sm, STATE_WRITING);
            break;
            
        case STATE_WRITING:
            if (sm->buffer_len > 0) {
                if (fwrite(sm->buffer, 1, sm->buffer_len, stdout) == sm->buffer_len) {
                    printf("\n");
                    sm->buffer_len = 0;
                    TRANSITION(sm, STATE_IDLE);
                } else {
                    sm->error_code = 2;
                    TRANSITION(sm, STATE_ERROR);
                }
            } else {
                TRANSITION(sm, STATE_IDLE);
            }
            break;
            
        case STATE_ERROR:
            fprintf(stderr, "Error %d occurred\n", sm->error_code);
            state_machine_init(sm);
            break;
            
        default:
            sm->error_code = 3;
            TRANSITION(sm, STATE_ERROR);
            break;
    }
    
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN];
    size_t input_len;
    
    printf("Enter text (empty line to exit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[--input_len] = '\0';
        }
        
        if (input_len == 0) {
            break;
        }
        
        if (state_machine_process(&sm, input, input_len) != 0) {
            fprintf(stderr, "Input validation failed\n");
            continue;
        }
        
        if (sm.state == STATE_IDLE) {
            printf("Ready for next input:\n");
        }
    }
    
    if (sm.state != STATE_IDLE && sm.state != STATE_ERROR) {
        state_machine_process(&sm, "", 0);
    }
    
    return 0;
}