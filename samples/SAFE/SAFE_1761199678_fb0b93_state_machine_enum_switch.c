//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
};

int main(void) {
    char buffer[256];
    char word[256];
    char number[256];
    int word_len = 0;
    int number_len = 0;
    int total_words = 0;
    int total_numbers = 0;
    enum state current_state = STATE_START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len == 0 || buffer[0] == '\n') {
        printf("Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len && current_state != STATE_END && current_state != STATE_ERROR; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\n' || c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    if (word_len < 255) {
                        word[word_len++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isdigit(c)) {
                    if (number_len < 255) {
                        number[number_len++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (!isspace(c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\n' || c == '\0') {
                    if (word_len > 0) {
                        word[word_len] = '\0';
                        total_words++;
                        printf("Found word: %s\n", word);
                        word_len = 0;
                    }
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    if (word_len < 255) {
                        word[word_len++] = c;
                    }
                } else if (isspace(c)) {
                    if (word_len > 0) {
                        word[word_len] = '\0';
                        total_words++;
                        printf("Found word: %s\n", word);
                        word_len = 0;
                    }
                    current_state = STATE_START;
                } else if (isdigit(c)) {
                    if (word_len > 0) {
                        word[word_len] = '\0';
                        total_words++;
                        printf("Found word: %s\n", word);
                        word_len = 0;
                    }
                    if (number_len < 255) {
                        number[number_len++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\n' || c == '\0') {
                    if (number_len > 0) {
                        number[number_len] = '\0';
                        total_numbers++;
                        printf("Found number: %s\n", number);
                        number_len = 0;
                    }
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    if (number_len < 255) {
                        number[number_len++] = c;
                    }
                } else if (isspace(c)) {
                    if (number_len > 0) {
                        number[number_len] = '\0';
                        total_numbers++;
                        printf("Found number: %s\n", number);
                        number_len = 0;
                    }
                    current_state = STATE_START;
                } else if (isalpha(c)) {
                    if (number_len > 0) {
                        number[number_len] = '\0';
                        total_numbers++;
                        printf("Found number: %s\n", number);
                        number_len = 0;
                    }
                    if (word_len < 255) {
                        word[word_len++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                printf("Invalid character encountered\n");
                break;
        }
    }
    
    if (current_state == STATE_READING_WORD && word_len > 0) {
        word[word_len] = '\0';
        total_words++;
        printf("Found word: %s\n", word);
    } else if (current_state == STATE_READING_NUMBER && number_len > 0) {
        number[number_len] = '\0';
        total_numbers++;
        printf("Found number: %s\n", number);
    }
    
    printf("Total words: %d\n", total_words);
    printf("Total numbers: %d\n", total_numbers);
    
    return 0;
}