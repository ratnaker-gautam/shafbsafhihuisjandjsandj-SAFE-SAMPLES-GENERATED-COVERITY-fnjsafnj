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
    STATE_COMPLETE,
    STATE_ERROR
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    unsigned int word_count;
    unsigned int char_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->char_count = 0;
}

int state_machine_transition(struct StateMachine *sm, char input) {
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == EOF) {
                sm->current_state = STATE_COMPLETE;
            } else if (isalpha(input)) {
                sm->buffer[sm->buffer_pos++] = input;
                sm->char_count++;
                sm->current_state = STATE_READING;
            } else if (isspace(input)) {
                sm->current_state = STATE_IDLE;
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING:
            if (input == EOF) {
                if (sm->buffer_pos > 0) {
                    sm->word_count++;
                    sm->buffer_pos = 0;
                }
                sm->current_state = STATE_COMPLETE;
            } else if (isalpha(input)) {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                    sm->char_count++;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (isspace(input)) {
                if (sm->buffer_pos > 0) {
                    sm->word_count++;
                    sm->buffer_pos = 0;
                }
                sm->current_state = STATE_IDLE;
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            sm->current_state = STATE_IDLE;
            break;
            
        case STATE_COMPLETE:
            break;
            
        case STATE_ERROR:
            break;
    }
    
    return 0;
}

int process_input(struct StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int ch;
    while ((ch = getchar()) != EOF) {
        if (state_machine_transition(sm, (char)ch) != 0) {
            return -1;
        }
        
        if (sm->current_state == STATE_ERROR) {
            printf("Error: Invalid input detected\n");
            return -1;
        }
    }
    
    if (state_machine_transition(sm, EOF) != 0) {
        return -1;
    }
    
    return 0;
}

void print_results(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("\nAnalysis Results:\n");
    printf("Words: %u\n", sm->word_count);
    printf("Characters: %u\n", sm->char_count);
    printf("Final State: ");
    
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("IDLE\n");
            break;
        case STATE_READING:
            printf("READING\n");
            break;
        case STATE_PROCESSING:
            printf("PROCESSING\n");
            break;
        case STATE_COMPLETE:
            printf("COMPLETE\n");
            break;
        case STATE_ERROR:
            printf("ERROR\n");
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    if (process_input(&sm) != 0) {
        return EXIT_FAILURE;
    }
    
    print_results(&sm);
    
    return EXIT_SUCCESS;
}