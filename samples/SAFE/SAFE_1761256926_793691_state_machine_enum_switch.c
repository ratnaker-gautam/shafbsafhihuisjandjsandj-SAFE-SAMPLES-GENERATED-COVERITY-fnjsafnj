//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int number = 0;
    int word_len = 0;
    int total_words = 0;
    int total_numbers = 0;
    int sum_numbers = 0;
    enum state current_state = STATE_START;
    
    printf("Enter text (words and numbers, max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word[word_len++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isdigit((unsigned char)c)) {
                    number = c - '0';
                    current_state = STATE_READING_NUMBER;
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || c == ' ' || c == '\t') {
                    if (word_len > 0 && word_len < 256) {
                        word[word_len] = '\0';
                        total_words++;
                        printf("Word: %s\n", word);
                        word_len = 0;
                    }
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word[word_len++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || c == ' ' || c == '\t') {
                    total_numbers++;
                    sum_numbers += number;
                    printf("Number: %d\n", number);
                    number = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    if (number <= (INT_MAX - digit) / 10) {
                        number = number * 10 + digit;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                printf("Invalid input format\n");
                return 1;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }
    
    printf("Total words: %d\n", total_words);
    printf("Total numbers: %d\n", total_numbers);
    if (total_numbers > 0) {
        printf("Sum of numbers: %d\n", sum_numbers);
    }
    
    return 0;
}