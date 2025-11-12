//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    PROCESS_NUMBER,
    SKIP_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char input_buffer[1024];
    char word_buffer[256];
    int word_len = 0;
    int number_value = 0;
    int has_number = 0;
    int has_word = 0;
    int char_count = 0;
    int i = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input_buffer)) {
        printf("Input too long\n");
        return 1;
    }
    
    while (i < input_len && current_state != FINISHED && current_state != ERROR) {
        char c = input_buffer[i];
        
        switch (current_state) {
            case INIT:
                if (isalnum((unsigned char)c)) {
                    current_state = READ_CHAR;
                    i--;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIP_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_CHAR:
                if (isalpha((unsigned char)c)) {
                    if (word_len < (int)sizeof(word_buffer) - 1) {
                        word_buffer[word_len++] = c;
                        current_state = PROCESS_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    current_state = PROCESS_NUMBER;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIP_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                if (isalpha((unsigned char)c)) {
                    if (word_len < (int)sizeof(word_buffer) - 1) {
                        word_buffer[word_len++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    word_buffer[word_len] = '\0';
                    has_word = 1;
                    word_len = 0;
                    current_state = SKIP_SPACE;
                } else if (isdigit((unsigned char)c)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (number_value <= (INT_MAX - (c - '0')) / 10) {
                        number_value = number_value * 10 + (c - '0');
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    has_number = 1;
                    current_state = SKIP_SPACE;
                } else if (isalpha((unsigned char)c)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIP_SPACE:
                if (!isspace((unsigned char)c)) {
                    if (isalnum((unsigned char)c)) {
                        current_state = READ_CHAR;
                        i--;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
        
        i++;
        char_count++;
        
        if (char_count > 10000) {
            current_state = ERROR;
        }
    }
    
    if (current_state == PROCESS_WORD && word_len > 0) {
        word_buffer[word_len] = '\0';
        has_word = 1;
    } else if (current_state == PROCESS_NUMBER) {
        has_number = 1;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Processing complete.\n");
    if (has_word) {
        printf("Found word: %s\n", word_buffer);
    }
    if (has_number) {
        printf("Found number: %d\n", number_value);
    }
    printf("Total characters processed: %d\n", char_count);
    
    return 0;
}