//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 50

#define TRANSITION(sm, new_state) do { (sm)->state = (new_state); } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
    int data_value;
} state_machine_t;

static void state_idle(state_machine_t *sm) {
    printf("Enter command (r=read, q=quit): ");
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    if (input[0] == 'r') {
        TRANSITION(sm, STATE_READING);
    } else if (input[0] == 'q') {
        exit(0);
    } else {
        printf("Invalid command\n");
    }
}

static void state_reading(state_machine_t *sm) {
    printf("Enter data (max %d chars): ", MAX_INPUT_LEN);
    char input[MAX_INPUT_LEN + 2];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[--len] = '\0';
    }
    
    if (!CHECK_BOUNDS(len, MAX_BUFFER)) {
        printf("Input too long\n");
        TRANSITION(sm, STATE_IDLE);
        return;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        TRANSITION(sm, STATE_IDLE);
        return;
    }
    
    memcpy(sm->buffer, input, len + 1);
    sm->buffer_len = len;
    TRANSITION(sm, STATE_PROCESSING);
}

static void state_processing(state_machine_t *sm) {
    int sum = 0;
    int valid_chars = 0;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= '0' && sm->buffer[i] <= '9') {
            int digit = sm->buffer[i] - '0';
            if (sum <= (INT_MAX - digit)) {
                sum += digit;
                valid_chars++;
            }
        }
    }
    
    if (valid_chars > 0) {
        sm->data_value = sum;
        printf("Processed %d digits, sum: %d\n", valid_chars, sum);
        TRANSITION(sm, STATE_WRITING);
    } else {
        printf("No digits found\n");
        TRANSITION(sm, STATE_IDLE);
    }
}

static void state_writing(state_machine_t *sm) {
    printf("Writing result: %d\n", sm->data_value);
    printf("Save successful\n");
    TRANSITION(sm, STATE_IDLE);
}

static void state_error(state_machine_t *sm) {
    printf("Error occurred\n");
    TRANSITION(sm, STATE_IDLE);
}

static void init_state_machine(state_machine_t *sm) {
    sm->state = STATE_IDLE;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
    sm->data_value = 0;
}

int main(void) {
    state_machine_t sm;
    init_state_machine(&sm);
    
    typedef void (*state_handler_t)(state_machine_t*);
    state_handler_t handlers[] = {
        state_idle,
        state_reading,
        state_processing,
        state_writing,
        state_error
    };
    
    while (1) {
        if (sm.state >= 0 && sm.state < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            handlers[sm.state](&sm);
        } else {
            printf("Invalid state\n");
            break;
        }
    }
    
    return 0;
}