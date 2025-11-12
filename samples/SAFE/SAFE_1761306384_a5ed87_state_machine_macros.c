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

int add_word(WordBuffer* buffer, const char* word, int len) {
    if (buffer->word_count >= MAX_WORDS || len >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(buffer->words[buffer->word_count], word, len);
    buffer->words[buffer->word_count][len] = '\0';
    buffer->word_count++;
    return 1;
}

void process_words(WordBuffer* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            buffer->words[i][j] = TO_UPPER(buffer->words[i][j]);
        }
    }
}

void print_words(const WordBuffer* buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s\n", buffer->words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
    }
    
    initialize_buffer(&buffer);
    TRANSITION(STATE_READING);
    
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        if (current_state == STATE_READING) {
            if (IS_VALID_CHAR(input[i])) {
                if (word_len < MAX_WORD_LEN - 1) {
                    current_word[word_len++] = input[i];
                } else {
                    TRANSITION(STATE_ERROR);
                    break;
                }
            } else {
                if (word_len > 0) {
                    if (!add_word(&buffer, current_word, word_len)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    word_len = 0;
                }
            }
        }
    }
    
    if (current_state != STATE_ERROR && word_len > 0) {
        if (!add_word(&buffer, current_word, word_len)) {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_READING) {
        TRANSITION(STATE_PROCESSING);
    }
    
    if (current_state == STATE_PROCESSING) {
        process_words(&buffer);
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words:\n");
        print_words(&buffer);
    } else if (current_state == STATE_ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    return 0;
}