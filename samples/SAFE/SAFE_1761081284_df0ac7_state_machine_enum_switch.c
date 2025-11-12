//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    char current_token[256];
    int token_pos = 0;
    enum State state = START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (state) {
            case START:
                if (c == '\0') {
                    state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                        state = READING_NUMBER;
                    } else {
                        state = ERROR;
                    }
                } else if (isalpha((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                        state = READING_WORD;
                    } else {
                        state = ERROR;
                    }
                } else if (!isspace((unsigned char)c)) {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace((unsigned char)c) || c == '\0') {
                    current_token[token_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", current_token);
                    token_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace((unsigned char)c) || c == '\0') {
                    current_token[token_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", current_token);
                    token_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}