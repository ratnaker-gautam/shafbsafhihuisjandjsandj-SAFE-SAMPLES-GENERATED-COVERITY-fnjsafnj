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
    int current_index;
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
    if (input == NULL || buffer == NULL) return 0;
    
    buffer->word_count = 0;
    buffer->current_index = 0;
    
    const char* start = input;
    const char* end = input;
    
    while (*end && buffer->word_count < MAX_WORDS) {
        while (*start == ' ' && *start) start++;
        if (!*start) break;
        
        end = start;
        while (*end != ' ' && *end && (end - start) < MAX_WORD_LEN - 1) end++;
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(buffer->words[buffer->word_count], start, word_len);
            buffer->words[buffer->word_count][word_len] = '\0';
            buffer->word_count++;
        }
        
        start = end;
    }
    
    return buffer->word_count > 0;
}

void process_word(char* word) {
    if (word == NULL) return;
    
    for (int i = 0; word[i]; i++) {
        if (word[i] >= 'a' && word[i] <= 'z') {
            word[i] = word[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int state = STATE_IDLE;
    int result = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        state = STATE_ERROR;
        result = 1;
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (!validate_input(input)) {
            state = STATE_ERROR;
            result = 1;
        } else {
            state = STATE_READING;
        }
    }
    
    while (state != STATE_ERROR && state != STATE_IDLE) {
        switch (state) {
            case STATE_READING:
                if (parse_words(input, &buffer)) {
                    state = STATE_PROCESSING;
                } else {
                    state = STATE_ERROR;
                    result = 1;
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer.current_index < buffer.word_count) {
                    process_word(buffer.words[buffer.current_index]);
                    buffer.current_index++;
                    if (buffer.current_index >= buffer.word_count) {
                        state = STATE_WRITING;
                    }
                } else {
                    state = STATE_ERROR;
                    result = 1;
                }
                break;
                
            case STATE_WRITING:
                printf("Processed words:\n");
                for (int i = 0; i < buffer.word_count; i++) {
                    if (i < MAX_WORDS) {
                        printf("%s\n", buffer.words[i]);
                    }
                }
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_ERROR;
                result = 1;
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
    }
    
    return result;
}