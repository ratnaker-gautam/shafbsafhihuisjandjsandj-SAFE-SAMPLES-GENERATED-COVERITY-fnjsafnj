//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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

int add_word(WordBuffer *buffer, const char *word) {
    if (buffer->word_count >= MAX_WORDS) return 0;
    if (strlen(word) >= MAX_WORD_LEN) return 0;
    
    strncpy(buffer->words[buffer->word_count], word, MAX_WORD_LEN - 1);
    buffer->words[buffer->word_count][MAX_WORD_LEN - 1] = '\0';
    buffer->word_count++;
    return 1;
}

void process_words(WordBuffer *buffer) {
    for (int i = 0; i < buffer->word_count; i++) {
        for (int j = 0; buffer->words[i][j] != '\0'; j++) {
            if (buffer->words[i][j] >= 'a' && buffer->words[i][j] <= 'z') {
                buffer->words[i][j] = buffer->words[i][j] - 'a' + 'A';
            }
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
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordBuffer word_buffer;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    initialize_buffer(&word_buffer);
    memset(input, 0, sizeof(input));
    memset(current_word, 0, sizeof(current_word));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len) {
                    char c = input[i];
                    if (IS_VALID_CHAR(c)) {
                        if (word_pos < MAX_WORD_LEN - 1) {
                            current_word[word_pos++] = c;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (word_pos > 0) {
                        current_word[word_pos] = '\0';
                        if (!add_word(&word_buffer, current_word)) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            word_pos = 0;
                            memset(current_word, 0, sizeof(current_word));
                        }
                    }
                } else if (word_pos > 0) {
                    current_word[word_pos] = '\0';
                    if (!add_word(&word_buffer, current_word)) {
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                process_words(&word_buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed words: ");
                print_words(&word_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Processing failed\n");
                return 1;
                
            case STATE_IDLE:
                break;
        }
    }
    
    if (current_state == STATE_IDLE) {
        printf("Processing completed successfully\n");
    }
    
    return 0;
}