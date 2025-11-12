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
    char data[512];
    size_t length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->char_count = 0;
    sm->line_count = 0;
}

int validate_input(const struct InputData *input) {
    if (input == NULL) return 0;
    if (input->length >= sizeof(input->data)) return 0;
    return 1;
}

void process_character(struct StateMachine *sm, char c) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (c != '\0') {
                sm->current_state = STATE_READING;
                process_character(sm, c);
            }
            break;
            
        case STATE_READING:
            if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = c;
                sm->char_count++;
                
                if (c == '\n') {
                    sm->line_count++;
                    sm->current_state = STATE_PROCESSING;
                } else if (isspace((unsigned char)c)) {
                    sm->current_state = STATE_PROCESSING;
                }
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0) {
                int in_word = 0;
                for (size_t i = 0; i < sm->buffer_pos; i++) {
                    if (!isspace((unsigned char)sm->buffer[i])) {
                        if (!in_word) {
                            sm->word_count++;
                            in_word = 1;
                        }
                    } else {
                        in_word = 0;
                    }
                }
            }
            
            sm->buffer_pos = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            
            if (c == '\0') {
                sm->current_state = STATE_DONE;
            } else {
                sm->current_state = STATE_READING;
                process_character(sm, c);
            }
            break;
            
        case STATE_WRITING:
            break;
            
        case STATE_ERROR:
            break;
            
        case STATE_DONE:
            break;
    }
}

void execute_state_machine(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || !validate_input(input)) {
        if (sm != NULL) sm->current_state = STATE_ERROR;
        return;
    }
    
    sm->current_state = STATE_IDLE;
    
    for (size_t i = 0; i < input->length; i++) {
        if (sm->current_state == STATE_ERROR || sm->current_state == STATE_DONE) {
            break;
        }
        process_character(sm, input->data[i]);
    }
    
    if (sm->current_state == STATE_READING || sm->current_state == STATE_PROCESSING) {
        process_character(sm, '\0');
    }
}

struct OutputData {
    int word_count;
    int char_count;
    int line_count;
    enum State final_state;
};

struct OutputData get_results(const struct StateMachine *sm) {
    struct OutputData output = {0, 0, 0, STATE_ERROR};
    if (sm != NULL) {
        output.word_count = sm->word_count;
        output.char_count = sm->char_count;
        output.line_count = sm->line_count;
        output.final_state = sm->current_state;
    }
    return output;
}

int main(void) {
    struct InputData input;
    struct StateMachine sm;
    struct OutputData results;
    
    printf("Enter text (max %zu characters): ", sizeof(input.data) - 1);
    
    if (fgets(input.data, sizeof(input.data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.length = strlen(input.data);
    if (input.length > 0 && input.data[input.length - 1] == '\n') {
        input.data[input.length - 1] = '\0';
        input.length--;
    }
    
    state_machine_init(&sm);
    execute_state_machine(&sm, &input);
    results = get_results(&sm);
    
    if (results.final_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", results.word_count);
    printf("Characters: %d\n", results.char_count);
    printf("Lines: %d\n", results.line_count);
    
    return 0;
}