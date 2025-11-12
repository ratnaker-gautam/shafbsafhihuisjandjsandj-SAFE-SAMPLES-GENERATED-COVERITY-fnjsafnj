//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
    int error_code;
};

struct InputHandler {
    char input_line[512];
    size_t input_pos;
    int has_more_input;
};

struct OutputHandler {
    char output_line[512];
    size_t output_pos;
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->line_count = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void input_handler_init(struct InputHandler *ih) {
    ih->input_pos = 0;
    ih->has_more_input = 1;
    memset(ih->input_line, 0, sizeof(ih->input_line));
}

void output_handler_init(struct OutputHandler *oh) {
    oh->output_pos = 0;
    memset(oh->output_line, 0, sizeof(oh->output_line));
}

int input_handler_read_line(struct InputHandler *ih) {
    if (!ih->has_more_input) {
        return 0;
    }
    
    if (fgets(ih->input_line, sizeof(ih->input_line), stdin) == NULL) {
        ih->has_more_input = 0;
        return 0;
    }
    
    size_t len = strlen(ih->input_line);
    if (len > 0 && ih->input_line[len - 1] == '\n') {
        ih->input_line[len - 1] = '\0';
    }
    
    ih->input_pos = 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void state_machine_transition(struct StateMachine *sm, enum State new_state) {
    sm->current_state = new_state;
}

int state_machine_process_idle(struct StateMachine *sm, struct InputHandler *ih) {
    if (input_handler_read_line(ih)) {
        state_machine_transition(sm, STATE_READING);
        return 1;
    }
    state_machine_transition(sm, STATE_DONE);
    return 0;
}

int state_machine_process_reading(struct StateMachine *sm, struct InputHandler *ih) {
    if (ih->input_pos >= sizeof(ih->input_line) || ih->input_line[ih->input_pos] == '\0') {
        state_machine_transition(sm, STATE_PROCESSING);
        return 1;
    }
    
    if (sm->buffer_pos >= sizeof(sm->buffer) - 1) {
        state_machine_transition(sm, STATE_ERROR);
        sm->error_code = 1;
        return 0;
    }
    
    sm->buffer[sm->buffer_pos++] = ih->input_line[ih->input_pos++];
    return 1;
}

int state_machine_process_processing(struct StateMachine *sm) {
    if (sm->buffer_pos == 0) {
        state_machine_transition(sm, STATE_IDLE);
        return 1;
    }
    
    sm->buffer[sm->buffer_pos] = '\0';
    int in_word = 0;
    
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (is_word_char(sm->buffer[i])) {
            if (!in_word) {
                sm->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    sm->line_count++;
    sm->buffer_pos = 0;
    state_machine_transition(sm, STATE_WRITING);
    return 1;
}

int state_machine_process_writing(struct StateMachine *sm, struct OutputHandler *oh) {
    int result = snprintf(oh->output_line, sizeof(oh->output_line),
                         "Line %d: %d words processed", sm->line_count, sm->word_count);
    
    if (result < 0 || (size_t)result >= sizeof(oh->output_line)) {
        state_machine_transition(sm, STATE_ERROR);
        sm->error_code = 2;
        return 0;
    }
    
    printf("%s\n", oh->output_line);
    sm->word_count = 0;
    state_machine_transition(sm, STATE_IDLE);
    return 1;
}

int state_machine_process_error(struct StateMachine *sm) {
    fprintf(stderr, "Error %d: State machine failure\n", sm->error_code);
    state_machine_transition(sm, STATE_DONE);
    return 0;
}

int state_machine_step(struct StateMachine *sm, struct InputHandler *ih, struct OutputHandler *oh) {
    switch (sm->current_state) {
        case STATE_IDLE:
            return state_machine_process_idle(sm, ih);
        case STATE_READING:
            return state_machine_process_reading(sm, ih);
        case STATE_PROCESSING:
            return state_machine_process_processing(sm);
        case STATE_WRITING:
            return