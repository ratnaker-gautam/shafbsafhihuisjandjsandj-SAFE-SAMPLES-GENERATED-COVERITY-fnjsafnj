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

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} state_machine_t;

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static int handle_idle(state_machine_t *sm, const char *input, size_t len) {
    if (sm == NULL || input == NULL) return 0;
    if (!validate_input(input, len)) {
        sm->error_code = 1;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (len > 0) {
        sm->buffer_len = (len < MAX_BUFFER_SIZE) ? len : MAX_BUFFER_SIZE - 1;
        memcpy(sm->buffer, input, sm->buffer_len);
        sm->buffer[sm->buffer_len] = '\0';
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

static int handle_reading(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 2;
        return 0;
    }
    TRANSITION(sm, STATE_PROCESSING);
    return 1;
}

static int handle_processing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 3;
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

static int handle_writing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 4;
        return 0;
    }
    
    if (fwrite(sm->buffer, sizeof(char), sm->buffer_len, stdout) != sm->buffer_len) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 5;
        return 0;
    }
    printf("\n");
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

static int handle_error(state_machine_t *sm) {
    if (sm == NULL) return 0;
    fprintf(stderr, "Error %d occurred\n", sm->error_code);
    state_machine_init(sm);
    return 0;
}

static int process_state_machine(state_machine_t *sm, const char *input, size_t len) {
    if (sm == NULL || input == NULL) return 0;
    
    switch (sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input, len);
        case STATE_READING:
            return handle_reading(sm);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            sm->error_code = 6;
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t sm;
    char input[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("Enter text (empty line to exit):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (!process_state_machine(&sm, input, len)) {
            if (sm.state == STATE_ERROR) {
                handle_error(&sm);
            }
        }