//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define TO_UPPER(c) ((c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c)

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_pos;
} WordBuffer;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void initialize_buffer(WordBuffer* buffer) {
    buffer->word_count = 0;
    buffer->current_pos = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

int process_character(WordBuffer* buffer, char c, int state) {
    switch(state) {
        case STATE_IDLE:
            if (IS_VALID_CHAR(c)) {
                if (buffer->word_count < MAX_WORDS && buffer->current_pos < MAX_WORD_LEN - 1) {
                    buffer->words[buffer->word_count][buffer->current_pos++] = TO_UPPER(c);
                    return STATE_READING;
                }
            }
            break;
            
        case STATE_READING:
            if (IS_VALID_CHAR(c)) {
                if (buffer->current_pos < MAX_WORD_LEN - 1) {
                    buffer->words[buffer->word_count][buffer->current_pos++] = TO_UPPER(c);
                    return STATE_READING;
                }
            } else {
                if (buffer->current_pos > 0) {
                    buffer->word_count++;
                    buffer->current_pos = 0;
                }
                return STATE_IDLE;
            }
            break;
            
        default:
            break;
    }
    return state;
}

void process_input(WordBuffer* buffer, const char* input) {
    int state = STATE_IDLE;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        state = process_character(buffer, input[i], state);
        if (state == STATE_ERROR) break;
    }
    
    if (state == STATE_READING && buffer->current_pos > 0) {
        buffer->word_count++;
    }
}

void print_results(const WordBuffer* buffer) {
    printf("Processed %d words:\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        if (strlen(buffer->words[i]) > 0) {
            printf("%d: %s\n", i + 1, buffer->words[i]);
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    initialize_buffer(&buffer);
    process_input(&buffer, input);
    
    if (buffer.word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    print_results(&buffer);
    
    return 0;
}