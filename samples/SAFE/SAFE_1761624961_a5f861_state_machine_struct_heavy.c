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

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->char_count = 0;
    sm->line_count = 0;
}

int state_machine_transition(struct StateMachine *sm, char input) {
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == '\0') {
                sm->current_state = STATE_DONE;
            } else if (input == '\n') {
                sm->line_count++;
                sm->char_count++;
            } else if (!isspace((unsigned char)input)) {
                sm->current_state = STATE_READING;
                sm->buffer[sm->buffer_pos++] = input;
                sm->char_count++;
            } else {
                sm->char_count++;
            }
            break;
            
        case STATE_READING:
            if (input == '\0') {
                sm->word_count++;
                sm->current_state = STATE_DONE;
            } else if (isspace((unsigned char)input)) {
                sm->word_count++;
                sm->current_state = STATE_IDLE;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                if (input == '\n') {
                    sm->line_count++;
                }
                sm->char_count++;
            } else if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = input;
                sm->char_count++;
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            sm->current_state = STATE_WRITING;
            break;
            
        case STATE_WRITING:
            sm->current_state = STATE_IDLE;
            break;
            
        case STATE_ERROR:
            return -1;
            
        case STATE_DONE:
            break;
    }
    
    return 0;
}

void state_machine_process_string(struct StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    if (len > 1024) return;
    
    for (size_t i = 0; i <= len; i++) {
        if (state_machine_transition(sm, input[i]) != 0) {
            break;
        }
    }
    
    if (sm->current_state == STATE_READING) {
        sm->word_count++;
    }
}

void print_statistics(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Text Statistics:\n");
    printf("Characters: %d\n", sm->char_count);
    printf("Words: %d\n", sm->word_count);
    printf("Lines: %d\n", sm->line_count);
    printf("Final State: ");
    
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE\n"); break;
        case STATE_READING: printf("READING\n"); break;
        case STATE_PROCESSING: printf("PROCESSING\n"); break;
        case STATE_WRITING: printf("WRITING\n"); break;
        case STATE_ERROR: printf("ERROR\n"); break;
        case STATE_DONE: printf("DONE\n"); break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input_buffer[1025];
    
    state_machine_init(&sm);
    
    printf("Enter text to analyze (max 1024 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    state_machine_process_string(&sm, input_buffer);
    
    if (sm.current_state == STATE_ERROR) {
        printf("Error: Buffer overflow occurred\n");
        return 1;
    }
    
    print_statistics(&sm);
    
    return 0;
}