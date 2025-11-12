//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

typedef struct {
    int numbers_found;
    int words_found;
    int errors_found;
} parser_stats_t;

static token_type_t get_next_token(const char *input, size_t *pos) {
    if (input[*pos] == '\0') return TOKEN_EOF;
    if (isdigit(input[*pos])) return TOKEN_DIGIT;
    if (isalpha(input[*pos])) return TOKEN_LETTER;
    if (isspace(input[*pos])) return TOKEN_SPACE;
    return TOKEN_INVALID;
}

static void process_state_machine(const char *input, parser_stats_t *stats) {
    parser_state_t current_state = STATE_START;
    size_t pos = 0;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        stats->errors_found++;
        return;
    }
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        token_type_t token = get_next_token(input, &pos);
        
        switch (current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    stats->numbers_found++;
                } else if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                    stats->words_found++;
                } else if (token == TOKEN_SPACE) {
                    pos++;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    pos++;
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    pos++;
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                    stats->errors_found++;
                }
                break;
                
            case STATE_ERROR:
                if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    pos++;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (pos > input_len) {
            current_state = STATE_ERROR;
            stats->errors_found++;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_stats_t stats = {0};
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_state_machine(input_buffer, &stats);
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", stats.numbers_found);
    printf("Words found: %d\n", stats.words_found);
    printf("Errors found: %d\n", stats.errors_found);
    
    return 0;
}