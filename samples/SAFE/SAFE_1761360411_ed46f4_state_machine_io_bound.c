//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

typedef struct {
    int numbers_found;
    int words_found;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} parser_data_t;

token_type_t get_next_token(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void process_state_machine(const char* input) {
    parser_state_t state = STATE_START;
    parser_data_t data = {0};
    size_t input_len = strlen(input);
    size_t pos = 0;
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    while (state != STATE_DONE && state != STATE_ERROR && pos <= input_len) {
        char current_char = input[pos];
        token_type_t token = get_next_token(current_char);
        
        switch (state) {
            case STATE_START:
                switch (token) {
                    case TOKEN_DIGIT:
                        state = STATE_READING_NUMBER;
                        data.current_number = current_char - '0';
                        break;
                    case TOKEN_LETTER:
                        state = STATE_READING_WORD;
                        data.current_word[0] = current_char;
                        data.current_word[1] = '\0';
                        break;
                    case TOKEN_SPACE:
                        break;
                    case TOKEN_EOF:
                        state = STATE_DONE;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                pos++;
                break;
                
            case STATE_READING_NUMBER:
                switch (token) {
                    case TOKEN_DIGIT:
                        if (data.current_number <= (INT_MAX - (current_char - '0')) / 10) {
                            data.current_number = data.current_number * 10 + (current_char - '0');
                        } else {
                            state = STATE_ERROR;
                        }
                        pos++;
                        break;
                    case TOKEN_SPACE:
                    case TOKEN_EOF:
                        data.numbers_found++;
                        printf("Found number: %d\n", data.current_number);
                        state = (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
                        pos++;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (token) {
                    case TOKEN_LETTER:
                        if (strlen(data.current_word) < MAX_INPUT_LEN - 1) {
                            size_t len = strlen(data.current_word);
                            data.current_word[len] = current_char;
                            data.current_word[len + 1] = '\0';
                        } else {
                            state = STATE_ERROR;
                        }
                        pos++;
                        break;
                    case TOKEN_SPACE:
                    case TOKEN_EOF:
                        data.words_found++;
                        printf("Found word: %s\n", data.current_word);
                        state = (token == TOKEN_EOF) ? STATE_DONE : STATE_START;
                        pos++;
                        break;
                    default:
                        state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                printf("Parser error at position %zu\n", pos);
                return;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_DONE) {
        printf("Parsing completed. Found %d numbers and %d words.\n", 
               data.numbers_found, data.words_found);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN && input[MAX_INPUT_LEN] != '\0' && input[MAX_INPUT_LEN] != '\n') {
        printf("Input too long\n");
        return 1;
    }
    
    process_state_machine(input);
    return 0;
}