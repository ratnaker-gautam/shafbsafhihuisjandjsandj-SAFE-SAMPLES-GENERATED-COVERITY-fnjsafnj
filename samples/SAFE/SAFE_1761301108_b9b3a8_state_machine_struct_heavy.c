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
    
    if (input_path) {
        sm->input_file = fopen(input_path, "r");
        if (!sm->input_file) {
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    
    if (output_path) {
        sm->output_file = fopen(output_path, "w");
        if (!sm->output_file) {
            if (sm->input_file) fclose(sm->input_file);
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    
    sm->current_state = STATE_READING;
}

void state_machine_cleanup(struct StateMachine *sm) {
    if (sm->input_file) fclose(sm->input_file);
    if (sm->output_file) fclose(sm->output_file);
}

int state_machine_process_char(struct StateMachine *sm, char c) {
    switch (sm->current_state) {
        case STATE_READING:
            if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                sm->buffer[sm->buffer_pos++] = c;
                if (c == '\n' || c == '\0') {
                    sm->buffer[sm->buffer_pos] = '\0';
                    sm->current_state = STATE_PROCESSING;
                }
            } else {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (sm->buffer_pos > 0) {
                int in_word = 0;
                for (size_t i = 0; i < sm->buffer_pos; i++) {
                    if (isalnum((unsigned char)sm->buffer[i])) {
                        if (!in_word) {
                            sm->word_count++;
                            in_word = 1;
                        }
                    } else {
                        in_word = 0;
                        if (sm->buffer[i] == '\n') {
                            sm->line_count++;
                        }
                    }
                }
                sm->current_state = STATE_WRITING;
            } else {
                sm->current_state = STATE_DONE;
            }
            break;
            
        case STATE_WRITING:
            if (sm->output_file) {
                if (fprintf(sm->output_file, "Line %d: %s", sm->line_count, sm->buffer) < 0) {
                    sm->current_state = STATE_ERROR;
                    return 0;
                }
            }
            sm->buffer_pos = 0;
            sm->current_state = STATE_READING;
            break;
            
        case STATE_ERROR:
        case STATE_DONE:
            return 0;
            
        default:
            sm->current_state = STATE_ERROR;
            return 0;
    }
    return 1;
}

int state_machine_run(struct StateMachine *sm) {
    if (sm->current_state == STATE_ERROR) return 0;
    
    int c;
    while ((c = fgetc(sm->input_file)) != EOF) {
        if (!state_machine_process_char(sm, (char)c)) {
            break;
        }
    }
    
    if (sm->current_state != STATE_ERROR) {
        if (sm->buffer_pos > 0) {
            state_machine_process_char(sm, '\0');
        }
        sm->current_state = STATE_DONE;
    }
    
    if (sm->output_file) {
        fprintf(sm->output_file, "Summary: %d words, %d lines\n", sm->word_count, sm->line_count);
    }
    
    printf("Processed %d words across %d lines\n", sm->word_count, sm->line_count);
    return sm->current_state == STATE_DONE;
}

int main(int argc, char *argv[]) {
    struct StateMachine sm;
    const char *input_file = "input.txt";
    const char *output_file = "output.txt";
    
    if (argc > 1) input_file = argv[1];
    if (argc > 2) output_file = argv[2];
    
    state_machine_init(&sm, input_file, output_file);
    
    if (sm.current_state == STATE_ERROR) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }
    
    int success = state_machine_run(&sm);
    state_machine_cleanup(&sm);
    
    return success ? 0 : 1;
}