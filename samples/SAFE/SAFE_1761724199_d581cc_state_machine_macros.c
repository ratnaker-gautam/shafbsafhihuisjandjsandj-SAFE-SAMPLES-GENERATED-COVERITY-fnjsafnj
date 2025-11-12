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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

static int validate_input(const char* input) {
    if (!input) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static int parse_words(const char* input, WordBuffer* buffer) {
    if (!input || !buffer) return 0;
    
    buffer->word_count = 0;
    const char* ptr = input;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr && buffer->word_count < MAX_WORDS) {
        if (*ptr == ' ') {
            if (word_len > 0) {
                if (word_len >= MAX_WORD_LEN) return 0;
                memcpy(buffer->words[buffer->word_count], current_word, word_len);
                buffer->words[buffer->word_count][word_len] = '\0';
                buffer->word_count++;
                word_len = 0;
            }
        } else {
            if (word_len >= MAX_WORD_LEN - 1) return 0;
            current_word[word_len++] = *ptr;
        }
        ptr++;
    }
    
    if (word_len > 0) {
        if (word_len >= MAX_WORD_LEN) return 0;
        memcpy(buffer->words[buffer->word_count], current_word, word_len);
        buffer->words[buffer->word_count][word_len] = '\0';
        buffer->word_count++;
    }
    
    return buffer->word_count > 0;
}

static void process_words(WordBuffer* buffer) {
    if (!buffer || buffer->word_count == 0) return;
    
    for (int i = 0; i < buffer->word_count; i++) {
        char* word = buffer->words[i];
        int len = strlen(word);
        for (int j = 0; j < len; j++) {
            if (word[j] >= 'a' && word[j] <= 'z') {
                word[j] = word[j] - 'a' + 'A';
            }
        }
    }
}

static void print_results(const WordBuffer* buffer) {
    if (!buffer || buffer->word_count == 0) return;
    
    printf("Processed words (%d):\n", buffer->word_count);
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%d: %s\n", i + 1, buffer->words[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordBuffer word_buffer;
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (!fgets(input, sizeof(input), stdin)) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING:
                if (!validate_input(input)) {
                    printf("Invalid input\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (!parse_words(input, &word_buffer)) {
                    printf("Failed to parse words\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    process_words(&word_buffer);
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                print_results(&word_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}