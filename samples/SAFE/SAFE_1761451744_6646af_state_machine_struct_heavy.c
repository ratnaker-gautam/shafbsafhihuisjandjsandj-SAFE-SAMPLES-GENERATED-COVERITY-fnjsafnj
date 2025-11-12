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
    FILE *input_file;
    FILE *output_file;
};

void state_machine_init(struct StateMachine *sm, const char *input_path, const char *output_path) {
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->word_count = 0;
    sm->line_count = 0;
    sm->input_file = NULL;
    sm->output_file = NULL;
    
    if (input_path && output_path) {
        sm->input_file = fopen(input_path, "r");
        if (!sm->input_file) {
            sm->current_state = STATE_ERROR;
            return;
        }
        
        sm->output_file = fopen(output_path, "w");
        if (!sm->output_file) {
            fclose(sm->input_file);
            sm->current_state = STATE_ERROR;
            return;
        }
        
        sm->current_state = STATE_READING;
    }
}

void state_machine_cleanup(struct StateMachine *sm) {
    if (sm->input_file) {
        fclose(sm->input_file);
        sm->input_file = NULL;
    }
    if (sm->output_file) {
        fclose(sm->output_file);
        sm->output_file = NULL;
    }
}

int state_machine_process_char(struct StateMachine *sm, char c) {
    switch (sm->current_state) {
        case STATE_READING:
            if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = c;
                if (c == '\n') {
                    sm->buffer[sm->buffer_pos] = '\0';
                    sm->current_state = STATE_PROCESSING;
                }
            } else {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            sm->line_count++;
            for (size_t i = 0; i < sm->buffer_pos; i++) {
                if (isspace((unsigned char)sm->buffer[i])) {
                    if (i > 0 && !isspace((unsigned char)sm->buffer[i-1])) {
                        sm->word_count++;
                    }
                }
            }
            if (sm->buffer_pos > 0 && !isspace((unsigned char)sm->buffer[sm->buffer_pos-1])) {
                sm->word_count++;
            }
            sm->current_state = STATE_WRITING;
            break;
            
        case STATE_WRITING:
            if (sm->output_file) {
                fprintf(sm->output_file, "Line %d: %s", sm->line_count, sm->buffer);
            }
            sm->buffer_pos = 0;
            sm->current_state = STATE_READING;
            break;
            
        case STATE_ERROR:
        case STATE_DONE:
        case STATE_IDLE:
            return 0;
    }
    return 1;
}

void state_machine_run(struct StateMachine *sm) {
    if (sm->current_state != STATE_READING) {
        return;
    }
    
    int c;
    while ((c = fgetc(sm->input_file)) != EOF) {
        if (!state_machine_process_char(sm, (char)c)) {
            break;
        }
    }
    
    if (sm->current_state == STATE_READING && sm->buffer_pos > 0) {
        sm->buffer[sm->buffer_pos] = '\0';
        sm->current_state = STATE_PROCESSING;
        state_machine_process_char(sm, '\0');
        state_machine_process_char(sm, '\0');
    }
    
    if (sm->current_state != STATE_ERROR) {
        sm->current_state = STATE_DONE;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    struct StateMachine sm;
    state_machine_init(&sm, argv[1], argv[2]);
    
    if (sm.current_state == STATE_ERROR) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }
    
    state_machine_run(&sm);
    
    if (sm.current_state == STATE_DONE) {
        printf("Processing complete: %d lines, %d words\n", sm.line_count, sm.word_count);
        if (sm.output_file) {
            fprintf(sm.output_file, "Summary: %d lines, %d words\n", sm.line_count, sm.word_count);
        }
    } else {
        fprintf(stderr, "Processing failed\n");
    }
    
    state_machine_cleanup(&sm);
    return 0;
}