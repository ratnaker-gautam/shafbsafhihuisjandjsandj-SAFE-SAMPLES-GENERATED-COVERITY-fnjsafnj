//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

token_type_t get_token_type(char c) {
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (c == ' ' || c == '\t') return TOKEN_SPACE;
    if (c == '\n') return TOKEN_NEWLINE;
    if (c == '\0') return TOKEN_EOF;
    return TOKEN_INVALID;
}

void process_input(const char* input) {
    parser_state_t state = STATE_START;
    int pos = 0;
    char current_char;
    int number_count = 0;
    int word_count = 0;
    
    while (state != STATE_END && state != STATE_ERROR) {
        if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
            current_char = '\0';
        } else {
            current_char = input[pos];
        }
        
        token_type_t token = get_token_type(current_char);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    word_count++;
                } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE) {
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_END;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE) {
                    state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_END;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_NEWLINE) {
                    state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_END;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", pos);
    } else {
        printf("Parsing completed successfully.\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN && input[MAX_INPUT_LEN - 1] != '\0') {
        printf("Warning: Input truncated to %d characters\n", MAX_INPUT_LEN);
        input[MAX_INPUT_LEN] = '\0';
    }
    
    process_input(input);
    return 0;
}