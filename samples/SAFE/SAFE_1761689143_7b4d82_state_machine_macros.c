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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_BUFFER 50

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buf_len;
    int data_value;
} state_machine_t;

static void state_idle(state_machine_t *sm, char input) {
    if (input == 'S') {
        sm->buf_len = 0;
        TRANSITION(sm, STATE_READING);
    }
}

static void state_reading(state_machine_t *sm, char input) {
    if (input == 'E') {
        if (sm->buf_len > 0) {
            TRANSITION(sm, STATE_PROCESSING);
        } else {
            TRANSITION(sm, STATE_ERROR);
        }
    } else if (CHECK_BOUNDS(sm->buf_len, MAX_BUFFER - 1) && input >= '0' && input <= '9') {
        sm->buffer[sm->buf_len++] = input;
    } else {
        TRANSITION(sm, STATE_ERROR);
    }
}

static void state_processing(state_machine_t *sm) {
    if (sm->buf_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    sm->buffer[sm->buf_len] = '\0';
    char *endptr;
    long value = strtol(sm->buffer, &endptr, 10);
    
    if (endptr == sm->buffer || *endptr != '\0' || value < 0 || value > 1000) {
        TRANSITION(sm, STATE_ERROR);
        return;
    }
    
    sm->data_value = (int)value;
    TRANSITION(sm, STATE_WRITING);
}

static void state_writing(state_machine_t *sm) {
    printf("Processed value: %d\n", sm->data_value);
    TRANSITION(sm, STATE_DONE);
}

static void state_error(state_machine_t *sm) {
    printf("Error: Invalid input sequence\n");
    TRANSITION(sm, STATE_DONE);
}

static void state_done(state_machine_t *sm) {
    printf("State machine completed\n");
}

static void process_input(state_machine_t *sm, char input) {
    switch (sm->state) {
        case STATE_IDLE:
            state_idle(sm, input);
            break;
        case STATE_READING:
            state_reading(sm, input);
            break;
        case STATE_PROCESSING:
            state_processing(sm);
            break;
        case STATE_WRITING:
            state_writing(sm);
            break;
        case STATE_ERROR:
            state_error(sm);
            break;
        case STATE_DONE:
            state_done(sm);
            break;
        default:
            TRANSITION(sm, STATE_ERROR);
            break;
    }
}

int main(void) {
    state_machine_t sm = {0};
    char input[MAX_INPUT_LEN];
    
    printf("Enter input sequence (S followed by digits then E): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len && sm.state != STATE_DONE; i++) {
        process_input(&sm, input[i]);
    }
    
    if (sm.state != STATE_DONE && sm.state != STATE_ERROR) {
        process_input(&sm, '\0');
    }
    
    return EXIT_SUCCESS;
}