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
    TOKEN_END,
    TOKEN_INVALID
} token_type_t;

token_type_t get_token_type(char c) {
    if (c == '\0') return TOKEN_END;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void process_input(const char* input) {
    parser_state_t state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    char current_char;
    
    printf("Processing input: '%s'\n", input);
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        current_char = input[pos];
        token_type_t token = get_token_type(current_char);
        
        switch (state) {
            case STATE_START:
                switch (token) {
                    case TOKEN_DIGIT:
                        state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_LETTER:
                        state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_SPACE:
                        break;
                    case TOKEN_END:
                        state = STATE_DONE;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (token) {
                    case TOKEN_DIGIT:
                        break;
                    case TOKEN_SPACE:
                        state = STATE_START;
                        break;
                    case TOKEN_END:
                        state = STATE_DONE;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (token) {
                    case TOKEN_LETTER:
                        break;
                    case TOKEN_SPACE:
                        state = STATE_START;
                        break;
                    case TOKEN_END:
                        state = STATE_DONE;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
        
        pos++;
        if (pos >= MAX_INPUT_LEN) {
            state = STATE_ERROR;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", pos);
    } else {
        printf("Successfully processed input\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Error: Input too long\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}