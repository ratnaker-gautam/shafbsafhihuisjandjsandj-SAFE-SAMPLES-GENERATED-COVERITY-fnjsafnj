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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (!(input[i] >= ' ' && input[i] <= '~')) return 0;
    }
    return 1;
}

int parse_words(const char* input, size_t len, WordBuffer* buffer) {
    if (buffer == NULL) return 0;
    buffer->word_count = 0;
    
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= len && input[i] != '\0'; i++) {
        if (buffer->word_count >= MAX_WORDS) break;
        
        char c = input[i];
        int is_space = (c == ' ' || c == '\t' || c == '\n');
        
        if (!in_word && !is_space) {
            word_start = i;
            in_word = 1;
        } else if (in_word && (is_space || i == len)) {
            size_t word_len = i - word_start;
            if (CHECK_BOUNDS(word_len, MAX_WORD_LEN)) {
                strncpy(buffer->words[buffer->word_count], &input[word_start], word_len);
                buffer->words[buffer->word_count][word_len] = '\0';
                buffer->word_count++;
            }
            in_word = 0;
        }
    }
    
    return buffer->word_count > 0;
}

void process_words(WordBuffer* buffer) {
    if (buffer == NULL) return;
    
    for (int i = 0; i < buffer->word_count; i++) {
        char* word = buffer->words[i];
        size_t len = strlen(word);
        
        for (size_t j = 0; j < len; j++) {
            if (word[j] >= 'a' && word[j] <= 'z') {
                word[j] = word[j] - 'a' + 'A';
            }
        }
    }
}

void output_words(const WordBuffer* buffer) {
    if (buffer == NULL) return;
    
    for (int i = 0; i < buffer->word_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", buffer->words[i]);
    }
    printf("\n");
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    WordBuffer word_buffer;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (!validate_input(input_buffer, len)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (parse_words(input_buffer, strlen(input_buffer), &word_buffer)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                output_words(&word_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}