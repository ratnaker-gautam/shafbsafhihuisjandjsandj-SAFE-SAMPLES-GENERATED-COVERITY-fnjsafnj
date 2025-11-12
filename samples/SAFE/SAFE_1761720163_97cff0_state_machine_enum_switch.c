//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    enum State current_state = START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    while (i <= len) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (token_pos > 0 && token_pos < sizeof(current_token)) {
                        current_token[token_pos] = '\0';
                        number_count++;
                        printf("Number: %s\n", current_token);
                        token_pos = 0;
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (token_pos > 0 && token_pos < sizeof(current_token)) {
                        current_token[token_pos] = '\0';
                        word_count++;
                        printf("Word: %s\n", current_token);
                        token_pos = 0;
                        current_state = START;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha((unsigned char)c)) {
                    if (token_pos < sizeof(current_token) - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (current_state == DONE) {
            break;
        }
        
        if (current_state != ERROR) {
            i++;
        }
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        if (token_pos > 0 && token_pos < sizeof(current_token)) {
            current_token[token_pos] = '\0';
            if (current_state == READING_NUMBER) {
                number_count++;
                printf("Number: %s\n", current_token);
            } else {
                word_count++;
                printf("Word: %s\n", current_token);
            }
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}