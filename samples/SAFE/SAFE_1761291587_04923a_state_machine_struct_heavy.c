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

struct InputData {
    char data[512];
    size_t length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->line_count = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_input(const struct InputData *input) {
    if (input == NULL) return 0;
    if (input->length >= sizeof(input->data)) return 0;
    return 1;
}

void process_idle_state(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || input == NULL) return;
    if (input->length > 0) {
        sm->current_state = STATE_READING;
    }
}

void process_reading_state(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || input == NULL) return;
    
    for (size_t i = 0; i < input->length; i++) {
        if (sm->buffer_pos >= sizeof(sm->buffer) - 1) {
            sm->current_state = STATE_ERROR;
            sm->error_code = 1;
            return;
        }
        
        sm->buffer[sm->buffer_pos++] = input->data[i];
        
        if (input->data[i] == '\n') {
            sm->current_state = STATE_PROCESSING;
            return;
        }
    }
}

void process_processing_state(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    int in_word = 0;
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (isspace((unsigned char)sm->buffer[i])) {
            if (in_word) {
                sm->word_count++;
                in_word = 0;
            }
            if (sm->buffer[i] == '\n') {
                sm->line_count++;
            }
        } else {
            in_word = 1;
        }
    }
    
    if (in_word) {
        sm->word_count++;
    }
    
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->current_state = STATE_WRITING;
}

void process_writing_state(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Words: %d, Lines: %d\n", sm->word_count, sm->line_count);
    sm->current_state = STATE_IDLE;
}

void process_error_state(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error: Buffer overflow detected\n");
    sm->current_state = STATE_DONE;
}

void process_done_state(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Processing complete.\n");
}

void state_machine_step(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || !validate_input(input)) {
        if (sm != NULL) {
            sm->current_state = STATE_ERROR;
            sm->error_code = 2;
        }
        return;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            process_idle_state(sm, input);
            break;
        case STATE_READING:
            process_reading_state(sm, input);
            break;
        case STATE_PROCESSING:
            process_processing_state(sm);
            break;
        case STATE_WRITING:
            process_writing_state(sm);
            break;
        case STATE_ERROR:
            process_error_state(sm);
            break;
        case STATE_DONE:
            process_done_state(sm);
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 3;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    struct InputData input;
    
    state_machine_init(&sm);
    
    printf("Enter text (empty line to finish):\n");
    
    while (sm.current_state != STATE_DONE) {
        if (fgets(input.data, sizeof(input.data), stdin) == NULL) {
            if (feof(stdin)) {
                input.data[0] = '\n';
                input.length = 1;
            } else {
                sm.current_state = STATE_ERROR;
                sm.error_code = 4;
                continue;
            }
        } else {
            input.length = strlen(input.data);
        }
        
        state_machine_step(&sm, &input);
        
        if (input.length == 1 && input.data[0] == '\n' && sm.current_state == STATE_IDLE) {
            sm.current_state = STATE_DONE;
        }
    }
    
    return sm.error_code;
}