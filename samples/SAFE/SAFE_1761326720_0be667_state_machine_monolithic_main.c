//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char buffer[256];
    char current_token[256];
    int token_len = 0;
    int line_number = 1;
    int char_count = 0;
    int number_count = 0;
    int word_count = 0;
    
    enum state current_state = START;
    
    printf("Enter text (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (token_len > 0) {
                        current_token[token_len] = '\0';
                        printf("Number %d: %s\n", ++number_count, current_token);
                        token_len = 0;
                    }
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else if (isspace((unsigned char)c)) {
                    if (token_len > 0) {
                        current_token[token_len] = '\0';
                        printf("Number %d: %s\n", ++number_count, current_token);
                        token_len = 0;
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (token_len > 0) {
                        current_token[token_len] = '\0';
                        printf("Word %d: %s\n", ++word_count, current_token);
                        token_len = 0;
                    }
                    current_state = FINISHED;
                } else if (isalnum((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else if (isspace((unsigned char)c)) {
                    if (token_len > 0) {
                        current_token[token_len] = '\0';
                        printf("Word %d: %s\n", ++word_count, current_token);
                        token_len = 0;
                    }
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                    current_state = READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Invalid character at position %zu\n", i);
                return 1;
        }
        
        if (c != '\0') {
            char_count++;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nStatistics:\n");
    printf("Characters processed: %d\n", char_count);
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    printf("Lines processed: %d\n", line_number);
    
    return 0;
}