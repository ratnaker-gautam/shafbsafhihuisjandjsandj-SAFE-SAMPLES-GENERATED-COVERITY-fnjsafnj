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
            if (input == '\n') {
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
            if (sm->buffer_pos >= sizeof(sm->buffer) - 1) {
                sm->current_state = STATE_ERROR;
                return -1;
            }
            
            if (isspace((unsigned char)input)) {
                sm->buffer[sm->buffer_pos] = '\0';
                sm->current_state = STATE_PROCESSING;
                sm->word_count++;
                sm->char_count++;
                
                if (input == '\n') {
                    sm->line_count++;
                }
            } else {
                sm->buffer[sm->buffer_pos++] = input;
                sm->char_count++;
            }
            break;
            
        case STATE_PROCESSING:
            if (input == '\n') {
                sm->line_count++;
                sm->char_count++;
                sm->current_state = STATE_IDLE;
            } else if (!isspace((unsigned char)input)) {
                sm->buffer_pos = 0;
                sm->buffer[sm->buffer_pos++] = input;
                sm->char_count++;
                sm->current_state = STATE_READING;
            } else {
                sm->char_count++;
            }
            break;
            
        case STATE_ERROR:
            return -1;
            
        case STATE_DONE:
            return 0;
            
        default:
            sm->current_state = STATE_ERROR;
            return -1;
    }
    
    return 0;
}

void state_machine_finalize(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_READING && sm->buffer_pos > 0) {
        sm->word_count++;
    }
    
    if (sm->buffer_pos > 0) {
        sm->current_state = STATE_WRITING;
    }
    
    sm->current_state = STATE_DONE;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (state_machine_transition(&sm, (char)c) != 0) {
            fprintf(stderr, "Error processing input\n");
            return 1;
        }
    }
    
    state_machine_finalize(&sm);
    
    printf("\nStatistics:\n");
    printf("Lines: %d\n", sm.line_count);
    printf("Words: %d\n", sm.word_count);
    printf("Characters: %d\n", sm.char_count);
    
    return 0;
}