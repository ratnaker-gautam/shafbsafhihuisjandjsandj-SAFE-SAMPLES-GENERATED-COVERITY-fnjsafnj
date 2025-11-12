//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char buffer[256];
    char current_token[256];
    int token_length = 0;
    int line_number = 1;
    int char_position = 0;
    enum State current_state = STATE_START;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        int c = getchar();
        char_position++;
        
        if (c == EOF) {
            if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
                if (token_length > 0 && token_length < 256) {
                    current_token[token_length] = '\0';
                    if (current_state == STATE_READING_NUMBER) {
                        printf("Line %d, Position %d: Number '%s'\n", line_number, char_position - token_length, current_token);
                    } else {
                        printf("Line %d, Position %d: Word '%s'\n", line_number, char_position - token_length, current_token);
                    }
                }
            }
            current_state = STATE_END;
            continue;
        }
        
        if (c == '\n') {
            line_number++;
            char_position = 0;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    token_length = 0;
                    if (token_length < 255) {
                        current_token[token_length++] = (char)c;
                    }
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    token_length = 0;
                    if (token_length < 255) {
                        current_token[token_length++] = (char)c;
                    }
                } else if (!isspace(c) && c != '\n') {
                    current_state = STATE_ERROR;
                    printf("Error: Invalid character '%c' at line %d, position %d\n", c, line_number, char_position);
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (token_length < 255) {
                        current_token[token_length++] = (char)c;
                    }
                } else if (isspace(c) || c == '\n') {
                    if (token_length > 0 && token_length < 256) {
                        current_token[token_length] = '\0';
                        printf("Line %d, Position %d: Number '%s'\n", line_number, char_position - token_length, current_token);
                    }
                    current_state = STATE_START;
                    token_length = 0;
                } else {
                    current_state = STATE_ERROR;
                    printf("Error: Invalid character '%c' in number at line %d, position %d\n", c, line_number, char_position);
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(c)) {
                    if (token_length < 255) {
                        current_token[token_length++] = (char)c;
                    }
                } else if (isspace(c) || c == '\n') {
                    if (token_length > 0 && token_length < 256) {
                        current_token[token_length] = '\0';
                        printf("Line %d, Position %d: Word '%s'\n", line_number, char_position - token_length, current_token);
                    }
                    current_state = STATE_START;
                    token_length = 0;
                } else {
                    current_state = STATE_ERROR;
                    printf("Error: Invalid character '%c' in word at line %d, position %d\n", c, line_number, char_position);
                }
                break;
                
            case STATE_ERROR:
                if (c == '\n') {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    printf("Processing completed successfully.\n");
    return EXIT_SUCCESS;
}