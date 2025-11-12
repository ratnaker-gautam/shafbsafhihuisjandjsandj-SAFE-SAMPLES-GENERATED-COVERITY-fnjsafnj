//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define BUFFER_FULL(buffer, pos) ((pos) >= MAX_BUFFER_SIZE - 1)

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_pos;
    int error_code;
} StateMachine;

void init_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_idle(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'S') {
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    if (input == 'E') {
        if (sm->buffer_pos > 0) {
            TRANSITION(sm, STATE_PROCESSING);
            return 1;
        } else {
            sm->error_code = 1;
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    } else if (input == 'Q') {
        TRANSITION(sm, STATE_IDLE);
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return 1;
    } else {
        if (!IS_VALID_CHAR(input)) {
            sm->error_code = 2;
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        if (BUFFER_FULL(sm->buffer, sm->buffer_pos)) {
            sm->error_code = 3;
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
        sm->buffer[sm->buffer_pos++] = input;
        return 1;
    }
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Processed: %s\n", sm->buffer);
    TRANSITION(sm, STATE_IDLE);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    switch (sm->error_code) {
        case 1:
            printf("Error: Empty buffer\n");
            break;
        case 2:
            printf("Error: Invalid character\n");
            break;
        case 3:
            printf("Error: Buffer overflow\n");
            break;
        default:
            printf("Error: Unknown error\n");
    }
    TRANSITION(sm, STATE_IDLE);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->error_code = 0;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (sm == NULL) return 0;
    switch (sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input);
        case STATE_READING:
            return handle_reading(sm, input);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            sm->error_code = 4;
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN];
    size_t input_len;
    
    init_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: S=Start, E=End, Q=Quit reading, Ctrl+D=Exit\n");
    printf("Enter text between S and E to process\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len == 0) {
            continue;
        }
        
        if (input_len == 1 && input[0] == 'Q' && sm.state == STATE_READING) {