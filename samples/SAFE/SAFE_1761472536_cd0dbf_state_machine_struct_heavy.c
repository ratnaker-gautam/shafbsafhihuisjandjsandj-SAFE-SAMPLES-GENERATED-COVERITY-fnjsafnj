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
    int char_count;
    int line_count;
};

struct InputData {
    char input_line[512];
    size_t input_len;
    int input_valid;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->char_count = 0;
    sm->line_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void input_data_init(struct InputData *data) {
    if (data == NULL) return;
    memset(data->input_line, 0, sizeof(data->input_line));
    data->input_len = 0;
    data->input_valid = 0;
}

int validate_input_line(const char *line, size_t max_len) {
    if (line == NULL) return 0;
    size_t len = strnlen(line, max_len);
    if (len >= max_len) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, struct InputData *input) {
    if (sm == NULL || input == NULL) return STATE_ERROR;
    if (input->input_valid && input->input_len > 0) {
        return STATE_READING;
    }
    return STATE_IDLE;
}

enum State handle_reading_state(struct StateMachine *sm, struct InputData *input) {
    if (sm == NULL || input == NULL) return STATE_ERROR;
    if (sm->buffer_pos + input->input_len >= sizeof(sm->buffer)) {
        return STATE_ERROR;
    }
    memcpy(sm->buffer + sm->buffer_pos, input->input_line, input->input_len);
    sm->buffer_pos += input->input_len;
    sm->char_count += input->input_len;
    return STATE_PROCESSING;
}

enum State handle_processing_state(struct StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
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
    return STATE_WRITING;
}

enum State handle_writing_state(struct StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    printf("Words: %d, Characters: %d, Lines: %d\n", 
           sm->word_count, sm->char_count, sm->line_count);
    return STATE_DONE;
}

enum State handle_error_state(struct StateMachine *sm) {
    if (sm == NULL) return STATE_ERROR;
    fprintf(stderr, "Error: Invalid state or input\n");
    return STATE_ERROR;
}

enum State transition_state(struct StateMachine *sm, struct InputData *input) {
    if (sm == NULL) return STATE_ERROR;
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, input);
        case STATE_READING:
            return handle_reading_state(sm, input);
        case STATE_PROCESSING:
            return handle_processing_state(sm);
        case STATE_WRITING:
            return handle_writing_state(sm);
        case STATE_ERROR:
            return handle_error_state(sm);
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct StateMachine sm;
    struct InputData input;
    state_machine_init(&sm);
    input_data_init(&input);
    
    printf("Enter text (empty line to finish):\n");
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        if (sm.current_state == STATE_IDLE || sm.current_state == STATE_READING) {
            input_data_init(&input);
            if (fgets(input.input_line, sizeof(input.input_line), stdin) == NULL) {
                break;
            }
            input.input_len = strnlen(input.input_line, sizeof(input.input_line));
            input.input_valid = validate_input_line(input.input_line, sizeof(input.input_line));
            
            if (input.input_valid && input.input_len == 1 && input.input_line[0] == '\n') {
                sm.current_state = STATE_PROCESSING;
                continue;
            }
        }
        
        sm.current_state = transition_state(&sm, &input);
    }
    
    if (sm.current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}