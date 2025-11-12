//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

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
    TOKEN_END,
    TOKEN_INVALID
} token_type_t;

token_type_t get_token_type(char c) {
    if (c == '\0' || c == '\n') return TOKEN_END;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void process_state_machine(const char* input) {
    parser_state_t current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    char current_char;
    
    printf("Processing input: %s\n", input);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        current_char = input[pos];
        token_type_t token = get_token_type(current_char);
        
        switch (current_state) {
            case STATE_START:
                switch (token) {
                    case TOKEN_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_LETTER:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_SPACE:
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (token) {
                    case TOKEN_DIGIT:
                        break;
                    case TOKEN_SPACE:
                        current_state = STATE_START;
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (token) {
                    case TOKEN_LETTER:
                        break;
                    case TOKEN_SPACE:
                        current_state = STATE_START;
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input at position %d\n", pos);
                return;
                
            case STATE_DONE:
                break;
        }
        
        pos++;
        if (pos >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Successfully processed input\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    } else {
        printf("Error: Input processing failed\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}