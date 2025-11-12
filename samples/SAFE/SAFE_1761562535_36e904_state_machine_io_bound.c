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
    TOKEN_NEWLINE,
    TOKEN_OTHER
} token_type_t;

static token_type_t classify_char(char c) {
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (c == ' ') return TOKEN_SPACE;
    if (c == '\n') return TOKEN_NEWLINE;
    return TOKEN_OTHER;
}

static void process_input(const char* input) {
    parser_state_t state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    char current_char;
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
            break;
        }
        
        current_char = input[pos];
        token_type_t token = classify_char(current_char);
        
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
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Error: Invalid input format at position %d\n", pos);
    } else {
        printf("Parsed %d numbers and %d words\n", number_count, word_count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN && input[MAX_INPUT_LEN - 1] != '\0') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}