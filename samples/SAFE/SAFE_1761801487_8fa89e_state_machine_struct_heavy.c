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
    char text[1024];
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
    if (input->length >= sizeof(input->text)) return 0;
    return 1;
}

void process_character(struct StateMachine *sm, char c) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (!isspace((unsigned char)c)) {
                sm->current_state = STATE_READING;
                sm->buffer[sm->buffer_pos++] = c;
                sm->char_count++;
            } else if (c == '\n') {
                sm->line_count++;
            }
            break;
            
        case STATE_READING:
            if (isspace((unsigned char)c)) {
                sm->current_state = STATE_PROCESSING;
                sm->word_count++;
                if (c == '\n') {
                    sm->line_count++;
                }
            } else {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = c;
                }
                sm->char_count++;
            }
            break;
            
        case STATE_PROCESSING:
            if (!isspace((unsigned char)c)) {
                sm->current_state = STATE_READING;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = c;
                }
                sm->char_count++;
            } else if (c == '\n') {
                sm->line_count++;
            }
            break;
            
        case STATE_ERROR:
        case STATE_DONE:
        case STATE_WRITING:
            break;
    }
}

void finalize_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_READING) {
        sm->word_count++;
    }
    sm->current_state = STATE_WRITING;
}

void print_statistics(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Text Statistics:\n");
    printf("Characters: %d\n", sm->char_count);
    printf("Words: %d\n", sm->word_count);
    printf("Lines: %d\n", sm->line_count);
}

int main(void) {
    struct StateMachine sm;
    struct InputData input;
    
    state_machine_init(&sm);
    
    printf("Enter text (max %zu characters):\n", sizeof(input.text) - 1);
    
    if (fgets(input.text, sizeof(input.text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input.length = strlen(input.text);
    if (input.length > 0 && input.text[input.length - 1] == '\n') {
        input.text[input.length - 1] = '\0';
        input.length--;
    }
    
    if (!validate_input(&input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    sm.current_state = STATE_IDLE;
    
    for (size_t i = 0; i < input.length; i++) {
        process_character(&sm, input.text[i]);
        
        if (sm.current_state == STATE_ERROR) {
            fprintf(stderr, "State machine error\n");
            return EXIT_FAILURE;
        }
    }
    
    finalize_processing(&sm);
    
    if (sm.current_state == STATE_WRITING) {
        print_statistics(&sm);
        sm.current_state = STATE_DONE;
    }
    
    return EXIT_SUCCESS;
}