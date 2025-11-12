//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = STATE_START;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->char_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

enum State handle_start(struct StateMachine *sm, char input) {
    if (input == '\n') {
        return STATE_DONE;
    }
    if (is_valid_char(input)) {
        if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
            sm->buffer[sm->buffer_pos++] = input;
            sm->char_count++;
        }
        return STATE_READING;
    }
    return STATE_ERROR;
}

enum State handle_reading(struct StateMachine *sm, char input) {
    if (input == '\n') {
        return STATE_PROCESSING;
    }
    if (is_valid_char(input)) {
        if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
            sm->buffer[sm->buffer_pos++] = input;
            sm->char_count++;
        } else {
            return STATE_ERROR;
        }
    } else {
        return STATE_ERROR;
    }
    return STATE_READING;
}

enum State handle_processing(struct StateMachine *sm) {
    int in_word = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isspace((unsigned char)sm->buffer[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                sm->word_count++;
                in_word = 1;
            }
        }
    }
    return STATE_OUTPUT;
}

enum State handle_output(struct StateMachine *sm) {
    printf("Characters: %d\n", sm->char_count);
    printf("Words: %d\n", sm->word_count);
    printf("Buffer contents: %s\n", sm->buffer);
    return STATE_DONE;
}

enum State handle_error(struct StateMachine *sm) {
    printf("Error: Invalid input detected\n");
    return STATE_DONE;
}

void state_machine_step(struct StateMachine *sm, char input) {
    switch (sm->current_state) {
        case STATE_START:
            sm->current_state = handle_start(sm, input);
            break;
        case STATE_READING:
            sm->current_state = handle_reading(sm, input);
            break;
        case STATE_PROCESSING:
            sm->current_state = handle_processing(sm);
            break;
        case STATE_OUTPUT:
            sm->current_state = handle_output(sm);
            break;
        case STATE_ERROR:
            sm->current_state = handle_error(sm);
            break;
        case STATE_DONE:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter text (max 255 chars, press Enter twice to finish):\n");
    
    while (sm.current_state != STATE_DONE) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        state_machine_step(&sm, (char)c);
    }
    
    return 0;
}