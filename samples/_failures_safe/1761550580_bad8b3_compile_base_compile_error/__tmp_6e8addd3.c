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
#define MAX_BUFFER_SIZE 256

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

static int process_buffer(state_machine_t *sm) {
    if (sm == NULL || sm->buffer_len == 0) return 0;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    return 1;
}

static void handle_idle(state_machine_t *sm, const char *input, size_t len) {
    if (sm == NULL || !validate_input(input, len)) {
        sm->error_code = 1;
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    if (len > 0) {
        size_t copy_len = len;
        if (copy_len >= MAX_BUFFER_SIZE) copy_len = MAX_BUFFER_SIZE - 1;
        memcpy(sm->buffer, input, copy_len);
        sm->buffer[copy_len] = '\0';
        sm->buffer_len = copy_len;
        TRANSITION(sm, STATE_READING);
    }
}

static void handle_reading(state_machine_t *sm) {
    if (sm == NULL || sm->buffer_len == 0) {
        sm->error_code = 2;
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    TRANSITION(sm, STATE_PROCESSING);
}

static void handle_processing(state_machine_t *sm) {
    if (sm == NULL) {
        sm->error_code = 3;
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    if (process_buffer(sm)) {
        TRANSITION(sm, STATE_WRITING);
    } else {
        sm->error_code = 4;
        TRANSITION(sm, STATE_ERROR);
    }
}

static void handle_writing(state_machine_t *sm) {
    if (sm == NULL || sm->buffer_len == 0) {
        sm->error_code = 5;
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    if (fwrite(sm->buffer, 1, sm->buffer_len, stdout) != sm->buffer_len) {
        sm->error_code = 6;
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    printf("\n");
    TRANSITION(sm, STATE_IDLE);
}

static void handle_error(state_machine_t *sm) {
    if (sm == NULL) return;
    fprintf(stderr, "Error %d occurred\n", sm->error_code);
    TRANSITION(sm, STATE_IDLE);
}

static void state_machine_step(state_machine_t *sm, const char *input, size_t len) {
    if (sm == NULL) return;
    
    switch (sm->state) {
        case STATE_IDLE:
            handle_idle(sm, input, len);
            break;
        case STATE_READING:
            handle_reading(sm);
            break;
        case STATE_PROCESSING:
            handle_processing(sm);
            break;
        case STATE_WRITING:
            handle_writing(sm);
            break;
        case STATE_ERROR:
            handle_error(sm);
            break;
        default:
            sm->error_code = 7;
            TRANSITION(sm, STATE_ERROR);
            break;
    }
}

int main(void) {
    state_machine_t sm;
    char input[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("Enter text (empty line to exit): ");
    fflush(stdout);
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        state_machine_step(&sm, input, len);
        
        if (sm.state == STATE_IDLE) {