//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int processed_count;
    int error_count;
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void state_idle(struct StateMachine *sm) {
    printf("Enter text (max 255 chars, empty line to finish): ");
    sm->current_state = STATE_READING;
}

void state_reading(struct StateMachine *sm) {
    if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        sm->current_state = STATE_DONE;
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_input_char(sm->buffer[i])) {
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    
    sm->buffer_pos = 0;
    sm->current_state = STATE_PROCESSING;
}

void state_processing(struct StateMachine *sm) {
    size_t len = strlen(sm->buffer);
    
    while (sm->buffer_pos < len) {
        if (islower(sm->buffer[sm->buffer_pos])) {
            sm->buffer[sm->buffer_pos] = toupper(sm->buffer[sm->buffer_pos]);
        } else if (isupper(sm->buffer[sm->buffer_pos])) {
            sm->buffer[sm->buffer_pos] = tolower(sm->buffer[sm->buffer_pos]);
        }
        sm->buffer_pos++;
    }
    
    sm->processed_count++;
    sm->current_state = STATE_WRITING;
}

void state_writing(struct StateMachine *sm) {
    printf("Processed: %s\n", sm->buffer);
    sm->current_state = STATE_IDLE;
}

void state_error(struct StateMachine *sm) {
    printf("Error: Invalid input detected\n");
    sm->error_count++;
    sm->current_state = STATE_IDLE;
}

void state_done(struct StateMachine *sm) {
    printf("Processing complete. Processed %d lines with %d errors.\n", 
           sm->processed_count, sm->error_count);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    while (sm.current_state != STATE_DONE) {
        switch (sm.current_state) {
            case STATE_IDLE:
                state_idle(&sm);
                break;
            case STATE_READING:
                state_reading(&sm);
                break;
            case STATE_PROCESSING:
                state_processing(&sm);
                break;
            case STATE_WRITING:
                state_writing(&sm);
                break;
            case STATE_ERROR:
                state_error(&sm);
                break;
            case STATE_DONE:
                break;
        }
        
        if (sm.processed_count > 100 || sm.error_count > 10) {
            printf("Limits exceeded, terminating.\n");
            break;
        }
    }
    
    return 0;
}