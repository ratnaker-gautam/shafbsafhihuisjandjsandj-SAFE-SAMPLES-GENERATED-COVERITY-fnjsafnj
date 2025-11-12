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
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define IS_TERMINATOR(c) ((c) == '\n' || (c) == '\0')

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_pos;
    size_t processed_count;
} state_machine_t;

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static int handle_idle(state_machine_t *sm, char input) {
    if (sm == NULL) return 0;
    
    if (IS_VALID_CHAR(input)) {
        if (sm->buffer_pos < MAX_BUFFER_SIZE - 1) {
            sm->buffer[sm->buffer_pos++] = input;
            TRANSITION(sm, STATE_READING);
            return 1;
        } else {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    }
    return 0;
}

static int handle_reading(state_machine_t *sm, char input) {
    if (sm == NULL) return 0;
    
    if (IS_VALID_CHAR(input)) {
        if (sm->buffer_pos < MAX_BUFFER_SIZE - 1) {
            sm->buffer[sm->buffer_pos++] = input;
            return 1;
        } else {
            TRANSITION(sm, STATE_ERROR);
            return 0;
        }
    } else if (IS_TERMINATOR(input)) {
        if (sm->buffer_pos > 0) {
            sm->buffer[sm->buffer_pos] = '\0';
            TRANSITION(sm, STATE_PROCESSING);
            return 1;
        } else {
            TRANSITION(sm, STATE_IDLE);
            return 0;
        }
    } else {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
}

static int handle_processing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_pos > 0) {
        for (size_t i = 0; i < sm->buffer_pos; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        sm->processed_count++;
        TRANSITION(sm, STATE_WRITING);
        return 1;
    } else {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
}

static int handle_writing(state_machine_t *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_pos > 0 && sm->buffer_pos < MAX_BUFFER_SIZE) {
        printf("Processed: %s\n", sm->buffer);
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        TRANSITION(sm, STATE_IDLE);
        return 1;
    } else {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
}

static int handle_error(state_machine_t *sm) {
    if (sm == NULL) return 0;
    printf("Error: Invalid input or buffer overflow\n");
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

static int process_input(state_machine_t *sm, char input) {
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
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    state_machine_t sm;
    char input[MAX_INPUT_LEN];
    
    state_machine_init(&sm);
    
    printf("Enter text (lowercase letters only, press Enter to process):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strnlen(input, sizeof(input));
        
        for (size_t i = 0; i < len; i++) {
            if (!process_input(&sm, input[i])) {
                break;
            }
        }
        
        if (sm.state == STATE_PROCESSING) {
            process_input(&sm, '\0');
        }
        
        printf("Enter more text or Ctrl+D to exit:\n");
    }
    
    printf("Total processed strings: %zu\n