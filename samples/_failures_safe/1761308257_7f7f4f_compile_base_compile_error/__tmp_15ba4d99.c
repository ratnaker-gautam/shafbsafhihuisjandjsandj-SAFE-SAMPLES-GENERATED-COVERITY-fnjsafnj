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
                return 0;
            } else if (input != ' ' && input != '\t') {
                sm->current_state = STATE_READING;
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                    sm->char_count++;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            }
            break;
            
        case STATE_READING:
            if (input == ' ' || input == '\t' || input == '\n') {
                sm->current_state = STATE_PROCESSING;
            } else {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                    sm->char_count++;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            }
            break;
            
        case STATE_PROCESSING:
            sm->buffer[sm->buffer_pos] = '\0';
            if (sm->buffer_pos > 0) {
                sm->word_count++;
            }
            sm->buffer_pos = 0;
            
            if (input == '\n') {
                sm->line_count++;
                sm->current_state = STATE_IDLE;
            } else if (input == ' ' || input == '\t') {
                sm->current_state = STATE_IDLE;
            } else {
                sm->current_state = STATE_READING;
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                    sm->char_count++;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            }
            break;
            
        case STATE_ERROR:
            return -1;
            
        default:
            sm->current_state = STATE_ERROR;
            return -1;
    }
    return 0;
}

int state_machine_finalize(struct StateMachine *sm) {
    if (sm == NULL) return -1;
    
    if (sm->current_state == STATE_READING) {
        sm->buffer[sm->buffer_pos] = '\0';
        if (sm->buffer_pos > 0) {
            sm->word_count++;
        }
    }
    sm->current_state = STATE_DONE;
    return 0;
}

struct TextStats {
    int total_words;
    int total_chars;
    int total_lines;
};

void text_stats_init(struct TextStats *stats) {
    if (stats == NULL) return;
    stats->total_words = 0;
    stats->total_chars = 0;
    stats->total_lines = 0;
}

void text_stats_accumulate(struct TextStats *stats, const struct StateMachine *sm) {
    if (stats == NULL || sm == NULL) return;
    stats->total_words += sm->word_count;
    stats->total_chars += sm->char_count;
    stats->total_lines += sm->line_count;
}

int process_input(struct StateMachine *sm, const char *input, size_t input_len) {
    if (sm == NULL || input == NULL) return -1;
    
    for (size_t i = 0; i < input_len; i++) {
        if (state_machine_transition(sm, input[i]) != 0) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    struct StateMachine sm;
    struct TextStats stats;
    
    state_machine_init(&sm);
    text_stats_init(&stats);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    char input_buffer[512];
    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
            len--;
        }
        
        if (process_input(&sm, input_buffer, len) != 0) {
            fprintf(stderr, "Error processing input\n");
            return 1;
        }
        
        if (len > 0) {
            process_input(&sm, " ", 1);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (state_machine_finalize