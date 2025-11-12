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

typedef struct {
    int numbers_found;
    int words_found;
    int total_tokens;
} parser_stats_t;

static token_type_t get_next_token(char c) {
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static void process_state_machine(const char* input, parser_stats_t* stats) {
    parser_state_t current_state = STATE_START;
    const char* ptr = input;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        token_type_t token = get_next_token(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    stats->numbers_found++;
                    stats->total_tokens++;
                } else if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                    stats->words_found++;
                    stats->total_tokens++;
                } else if (token == TOKEN_SPACE) {
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state != STATE_DONE && current_state != STATE_ERROR) {
            ptr++;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
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
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_state_machine(input, &stats);
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", stats.numbers_found);
    printf("Words found: %d\n", stats.words_found);
    printf("Total tokens: %d\n", stats.total_tokens);
    
    if (stats.total_tokens == 0) {
        printf("No valid tokens found in input.\n");
    }
    
    return 0;
}