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

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int current_word;
} WordBuffer;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

int parse_words(const char* input, WordBuffer* buffer) {
    if (buffer == NULL) return 0;
    buffer->word_count = 0;
    buffer->current_word = 0;
    
    const char* ptr = input;
    int in_word = 0;
    int word_len = 0;
    
    while (*ptr != '\0' && buffer->word_count < MAX_WORDS) {
        if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                buffer->words[buffer->word_count][word_len++] = *ptr;
            }
        } else {
            if (in_word) {
                buffer->words[buffer->word_count][word_len] = '\0';
                buffer->word_count++;
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && buffer->word_count < MAX_WORDS) {
        buffer->words[buffer->word_count][word_len] = '\0';
        buffer->word_count++;
    }
    
    return buffer->word_count > 0;
}

void process_word(char* word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (word[i] >= 'a' && word[i] <= 'z') {
            word[i] = word[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int state = STATE_IDLE;
    WordBuffer buffer;
    int result;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        state = STATE_ERROR;
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        state = STATE_READING;
    }
    
    while (state != STATE_ERROR && state != STATE_IDLE) {
        switch (state) {
            case STATE_READING:
                if (!validate_input(input)) {
                    state = STATE_ERROR;
                    break;
                }
                result = parse_words(input, &buffer);
                if (!result) {
                    state = STATE_ERROR;
                } else {
                    state = STATE_PROCESSING;
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer.current_word < buffer.word_count) {
                    process_word(buffer.words[buffer.current_word]);
                    buffer.current_word++;
                } else {
                    state = STATE_WRITING;
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words:\n");
                for (int i = 0; i < buffer.word_count; i++) {
                    printf("%s\n", buffer.words[i]);
                }
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    return 0;
}