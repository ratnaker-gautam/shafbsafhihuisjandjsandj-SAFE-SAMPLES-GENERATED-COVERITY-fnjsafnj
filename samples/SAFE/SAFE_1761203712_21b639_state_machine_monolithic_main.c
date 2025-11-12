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
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    while (i <= input_len && current_state != FINISHED && current_state != ERROR) {
        char c = (i < input_len) ? input_buffer[i] : '\0';
        
        switch (current_state) {
            case INIT:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIP_SPACE;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word_buffer[word_len++] = tolower((unsigned char)c);
                        has_word = 1;
                        current_state = PROCESS_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    has_number = 1;
                    current_state = PROCESS_NUMBER;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READ_CHAR:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIP_SPACE;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word_buffer[word_len++] = tolower((unsigned char)c);
                        has_word = 1;
                        current_state = PROCESS_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    has_number = 1;
                    current_state = PROCESS_NUMBER;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (word_len > 0) {
                        word_buffer[word_len] = '\0';
                        printf("Word: %s\n", word_buffer);
                        word_len = 0;
                    }
                    current_state = (c == '\0') ? FINISHED : SKIP_SPACE;
                } else if (isalpha((unsigned char)c)) {
                    if (word_len < 255) {
                        word_buffer[word_len++] = tolower((unsigned char)c);
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (has_number) {
                        printf("Number: %d\n", number_value);
                        number_value = 0;
                        has_number = 0;
                    }
                    current_state = (c == '\0') ? FINISHED : SKIP_SPACE;
                } else if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    if (number_value <= (INT_MAX - digit) / 10) {
                        number_value = number_value * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIP_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    current_state = READ_CHAR;
                    i--;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
        
        i++;
        char_count++;
        
        if (char_count > 2048) {
            current_state = ERROR;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (has_word && word_len > 0) {
        word_buffer[word_len] = '\0';
        printf("Word: %s\n", word_buffer);
    }
    
    if (has_number) {
        printf("Number: %d\n", number_value);
    }
    
    if (current_state != FINISHED) {
        printf("Error: Processing incomplete\n");
        return 1;
    }
    
    return 0;
}