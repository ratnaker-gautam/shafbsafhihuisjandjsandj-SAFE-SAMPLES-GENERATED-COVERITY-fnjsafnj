//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    STATE_DONE
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

token_type_t get_next_token(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers_found = 0;
    int words_found = 0;
    int current_pos = 0;
    parser_state_t current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    printf("Processing input: '%s'\n", input);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        char current_char = input[current_pos];
        token_type_t token = get_next_token(current_char);
        
        switch (current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    numbers_found++;
                } else if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                    words_found++;
                } else if (token == TOKEN_SPACE) {
                    current_pos++;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    current_pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                    current_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    current_pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    current_state = STATE_START;
                    current_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Parse error at position %d\n", current_pos);
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (current_pos > (int)input_len) {
            current_state = STATE_DONE;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Failed to parse input\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", numbers_found);
    printf("Words found: %d\n", words_found);
    
    return 0;
}