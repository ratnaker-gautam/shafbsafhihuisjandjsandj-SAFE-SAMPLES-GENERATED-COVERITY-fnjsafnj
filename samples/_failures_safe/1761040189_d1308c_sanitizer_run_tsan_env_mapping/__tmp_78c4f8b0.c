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

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len) do { if ((len) >= MAX_BUFFER_SIZE) { TRANSITION(STATE_ERROR); return; } } while(0)

static char input_buffer[MAX_BUFFER_SIZE];
static size_t buffer_pos = 0;

static void state_idle(const char* input);
static void state_reading(const char* input);
static void state_processing(void);
static void state_writing(void);
static void state_error(void);

static int current_state = STATE_IDLE;

static void state_idle(const char* input) {
    if (input == NULL) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    if (strlen(input) > 0) {
        TRANSITION(STATE_READING);
        state_reading(input);
    }
}

static void state_reading(const char* input) {
    if (input == NULL) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    CHECK_BOUNDS(buffer_pos + input_len);
    
    for (size_t i = 0; i < input_len; i++) {
        if (buffer_pos >= MAX_BUFFER_SIZE) {
            TRANSITION(STATE_ERROR);
            return;
        }
        input_buffer[buffer_pos++] = input[i];
    }
    
    TRANSITION(STATE_PROCESSING);
    state_processing();
}

static void state_processing(void) {
    if (buffer_pos == 0) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    for (size_t i = 0; i < buffer_pos; i++) {
        if (input_buffer[i] >= 'a' && input_buffer[i] <= 'z') {
            input_buffer[i] = input_buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(STATE_WRITING);
    state_writing();
}

static void state_writing(void) {
    if (buffer_pos == 0) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    for (size_t i = 0; i < buffer_pos; i++) {
        putchar(input_buffer[i]);
    }
    putchar('\n');
    
    buffer_pos = 0;
    TRANSITION(STATE_IDLE);
}

static void state_error(void) {
    printf("ERROR: Invalid state transition or buffer overflow\n");
    buffer_pos = 0;
    TRANSITION(STATE_IDLE);
}

static void process_input(const char* input) {
    if (input == NULL) {
        TRANSITION(STATE_ERROR);
        return;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            state_idle(input);
            break;
        case STATE_READING:
            state_reading(input);
            break;
        case STATE_PROCESSING:
            state_processing();
            break;
        case STATE_WRITING:
            state_writing();
            break;
        case STATE_ERROR:
            state_error();
            break;
        default:
            TRANSITION(STATE_ERROR);
            break;
    }
}

int main(void) {
    char user_input[MAX_INPUT_LEN];
    
    printf("State Machine Processor (type 'quit' to exit)\n");
    
    while (1) {
        printf("Enter text: ");
        
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            TRANSITION(STATE_ERROR);
            continue;
        }
        
        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }
        
        if (strcmp(user_input, "quit") == 0) {
            break;
        }
        
        process_input(user_input);
    }
    
    return 0;
}