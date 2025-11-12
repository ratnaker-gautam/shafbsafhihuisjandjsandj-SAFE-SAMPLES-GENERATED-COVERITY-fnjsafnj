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
    int total_tokens;
} parser_stats_t;

token_type_t get_next_token(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

void process_token(parser_state_t *state, token_type_t token, parser_stats_t *stats) {
    switch (*state) {
        case STATE_START:
            if (token == TOKEN_DIGIT) {
                *state = STATE_READING_NUMBER;
                stats->numbers_found++;
                stats->total_tokens++;
            } else if (token == TOKEN_LETTER) {
                *state = STATE_READING_WORD;
                stats->words_found++;
                stats->total_tokens++;
            } else if (token == TOKEN_SPACE) {
                *state = STATE_START;
            } else if (token == TOKEN_EOF) {
                *state = STATE_DONE;
            } else {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (token == TOKEN_DIGIT) {
                *state = STATE_READING_NUMBER;
            } else if (token == TOKEN_SPACE) {
                *state = STATE_START;
            } else if (token == TOKEN_EOF) {
                *state = STATE_DONE;
            } else {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (token == TOKEN_LETTER) {
                *state = STATE_READING_WORD;
            } else if (token == TOKEN_SPACE) {
                *state = STATE_START;
            } else if (token == TOKEN_EOF) {
                *state = STATE_DONE;
            } else {
                *state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (token == TOKEN_SPACE) {
                *state = STATE_START;
            } else if (token == TOKEN_EOF) {
                *state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            break;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && input[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    parser_state_t state = STATE_START;
    parser_stats_t stats = {0, 0, 0};
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        token_type_t token = get_next_token(input[i]);
        process_token(&state, token, &stats);
        
        if (state == STATE_ERROR) {
            printf("Parser encountered error at position %zu\n", i + 1);
            break;
        }
    }
    
    if (state != STATE_ERROR) {
        process_token(&state, TOKEN_EOF, &stats);
    }
    
    if (state == STATE_DONE || state == STATE_ERROR) {
        printf("Parsing complete:\n");
        printf("Numbers found: %d\n", stats.numbers_found);
        printf("Words found: %d\n", stats.words_found);
        printf("Total tokens: %d\n", stats.total_tokens);
    }
    
    return 0;
}