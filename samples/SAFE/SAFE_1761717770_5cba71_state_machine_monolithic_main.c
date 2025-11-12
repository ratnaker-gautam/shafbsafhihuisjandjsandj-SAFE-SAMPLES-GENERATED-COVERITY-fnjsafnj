//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char buffer[256];
    char current_token[256];
    int token_len = 0;
    enum State current_state = START;
    int total_numbers = 0;
    int total_words = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(buffer);
    if (input_len == 0 || (input_len == 1 && buffer[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = (i < input_len) ? buffer[i] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    token_len = 0;
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    token_len = 0;
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else {
                    if (token_len > 0 && token_len < sizeof(current_token)) {
                        current_token[token_len] = '\0';
                        printf("Found number: %s\n", current_token);
                        total_numbers++;
                    }
                    
                    if (isalpha((unsigned char)c)) {
                        current_state = READING_WORD;
                        token_len = 0;
                        if (token_len < sizeof(current_token) - 1) {
                            current_token[token_len++] = c;
                        }
                    } else if (isspace((unsigned char)c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalnum((unsigned char)c)) {
                    if (token_len < sizeof(current_token) - 1) {
                        current_token[token_len++] = c;
                    }
                } else {
                    if (token_len > 0 && token_len < sizeof(current_token)) {
                        current_token[token_len] = '\0';
                        printf("Found word: %s\n", current_token);
                        total_words++;
                    }
                    
                    if (isdigit((unsigned char)c)) {
                        current_state = READING_NUMBER;
                        token_len = 0;
                        if (token_len < sizeof(current_token) - 1) {
                            current_token[token_len++] = c;
                        }
                    } else if (isspace((unsigned char)c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (!isspace((unsigned char)c)) {
                    if (isdigit((unsigned char)c)) {
                        current_state = READING_NUMBER;
                        token_len = 0;
                        if (token_len < sizeof(current_token) - 1) {
                            current_token[token_len++] = c;
                        }
                    } else if (isalpha((unsigned char)c)) {
                        current_state = READING_WORD;
                        token_len = 0;
                        if (token_len < sizeof(current_token) - 1) {
                            current_token[token_len++] = c;
                        }
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case END:
                break;
        }
        
        if (current_state == END) {
            break;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", total_numbers, total_words);
    return 0;
}