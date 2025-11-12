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

void initialize_buffer(WordBuffer *buffer) {
    buffer->word_count = 0;
    buffer->current_pos = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(buffer->words[i], 0, MAX_WORD_LEN);
    }
}

int add_word(WordBuffer *buffer, const char *word) {
    if (buffer->word_count >= MAX_WORDS) {
        return 0;
    }
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(buffer->words[buffer->word_count], word, MAX_WORD_LEN - 1);
    buffer->words[buffer->word_count][MAX_WORD_LEN - 1] = '\0';
    buffer->word_count++;
    return 1;
}

void process_words(WordBuffer *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            buffer->words[i][j] = TO_UPPER(buffer->words[i][j]);
        }
    }
}

void print_words(const WordBuffer *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        printf("%s ", buffer->words[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int current_state = STATE_IDLE;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    initialize_buffer(&buffer);
    memset(current_word, 0, MAX_WORD_LEN);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        if (len >= MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    for (int i = 0; current_state != STATE_ERROR && input[i] != '\0'; i++) {
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(input[i])) {
                    if (word_pos < MAX_WORD_LEN - 1) {
                        current_word[word_pos++] = input[i];
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (word_pos > 0) {
                        current_word[word_pos] = '\0';
                        if (!add_word(&buffer, current_word)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        word_pos = 0;
                        memset(current_word, 0, MAX_WORD_LEN);
                    }
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && word_pos > 0) {
        current_word[word_pos] = '\0';
        if (!add_word(&buffer, current_word)) {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_READING && buffer.word_count > 0) {
        TRANSITION(STATE_PROCESSING);
    } else if (current_state == STATE_READING) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state == STATE_PROCESSING) {
        process_words(&buffer);
        TRANSITION(STATE_WRITING);
    }
    
    if (current_state == STATE_WRITING) {
        printf("Processed words: ");
        print_words(&buffer);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}