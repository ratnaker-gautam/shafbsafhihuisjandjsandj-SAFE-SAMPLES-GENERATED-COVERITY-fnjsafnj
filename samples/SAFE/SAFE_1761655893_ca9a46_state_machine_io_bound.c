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

token_type_t classify_char(char c) {
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (c == ' ') return TOKEN_SPACE;
    if (c == '\n') return TOKEN_NEWLINE;
    return TOKEN_OTHER;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers_found = 0;
    int words_found = 0;
    int pos = 0;
    parser_state_t state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (pos < MAX_INPUT_LEN && input[pos] != '\0') {
        token_type_t token = classify_char(input[pos]);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    numbers_found++;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    words_found++;
                } else if (token == TOKEN_SPACE) {
                    state = STATE_START;
                } else if (token == TOKEN_OTHER) {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                } else if (token == TOKEN_SPACE) {
                    state = STATE_START;
                } else if (token == TOKEN_LETTER || token == TOKEN_OTHER) {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                } else if (token == TOKEN_SPACE) {
                    state = STATE_START;
                } else if (token == TOKEN_DIGIT || token == TOKEN_OTHER) {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                if (token == TOKEN_SPACE) {
                    state = STATE_START;
                }
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (state == STATE_ERROR) {
            printf("Invalid input format at position %d\n", pos);
            break;
        }
        
        pos++;
    }
    
    if (state != STATE_ERROR) {
        printf("Parsing completed successfully\n");
        printf("Numbers found: %d\n", numbers_found);
        printf("Words found: %d\n", words_found);
    } else {
        printf("Parsing failed due to invalid input\n");
    }
    
    return 0;
}