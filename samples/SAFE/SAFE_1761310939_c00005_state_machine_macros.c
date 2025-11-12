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

void initialize_buffer(WordBuffer *buffer) {
    buffer->word_count = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

int process_input(const char *input, WordBuffer *buffer) {
    if (input == NULL || buffer == NULL) return 0;
    
    int input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    char current_word[MAX_WORD_LEN] = {0};
    int word_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && c != ' ' && word_pos < MAX_WORD_LEN - 1) {
            if (!in_word) in_word = 1;
            current_word[word_pos++] = c;
        } else {
            if (in_word && buffer->word_count < MAX_WORDS) {
                current_word[word_pos] = '\0';
                if (!safe_strncpy(buffer->words[buffer->word_count], current_word, MAX_WORD_LEN)) {
                    return 0;
                }
                buffer->word_count++;
                word_pos = 0;
                in_word = 0;
                memset(current_word, 0, MAX_WORD_LEN);
            }
        }
    }
    return 1;
}

void print_words(const WordBuffer *buffer) {
    if (buffer == NULL) return;
    printf("Processed words (%d):\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%d: %s\n", i + 1, buffer->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1] = {0};
    WordBuffer buffer;
    
    initialize_buffer(&buffer);
    
    printf("Word Processor State Machine\n");
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        for (size_t i = 0; i < len; i++) {
            if (!is_valid_char(input[i]) && input[i] != '\n' && input[i] != '\0') {
                TRANSITION(STATE_ERROR);
                break;
            }
        }
    }
    
    if (current_state != STATE_ERROR) {
        TRANSITION(STATE_READING);
    }
    
    switch (current_state) {
        case STATE_READING:
            if (strlen(input) > 0) {
                TRANSITION(STATE_PROCESSING);
            } else {
                TRANSITION(STATE_ERROR);
            }
            break;
        default:
            break;
    }
    
    switch (current_state) {
        case STATE_PROCESSING:
            if (process_input(input, &buffer)) {
                TRANSITION(STATE_WRITING);
            } else {
                TRANSITION(STATE_ERROR);
            }
            break;
        default:
            break;
    }
    
    switch (current_state) {
        case STATE_WRITING:
            print_words(&buffer);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input or processing error\n");
            break;
        default:
            break;
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}