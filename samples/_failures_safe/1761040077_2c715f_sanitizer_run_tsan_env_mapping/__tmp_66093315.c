//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

static token_type_t get_next_token(char input, int *pos, int max_len) {
    if (*pos >= max_len) return TOKEN_EOF;
    
    char c = input;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (isspace(c)) return TOKEN_SPACE;
    
    return TOKEN_INVALID;
}

static parser_state_t handle_start_state(token_type_t token, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_DIGIT:
            stats->numbers_found++;
            return STATE_READING_NUMBER;
        case TOKEN_LETTER:
            stats->words_found++;
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            stats->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_reading_number(token_type_t token, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_DIGIT:
            return STATE_READING_NUMBER;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            stats->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(token_type_t token, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_LETTER:
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            stats->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_error_state(token_type_t token, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static void process_input(const char *input, int input_len, parser_stats_t *stats) {
    parser_state_t current_state = STATE_START;
    int pos = 0;
    
    while (current_state != STATE_END && pos < input_len) {
        token_type_t token = get_next_token(input[pos], &pos, input_len);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start_state(token, stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(token, stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(token, stats);
                break;
            case STATE_ERROR:
                current_state = handle_error_state(token, stats);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (token != TOKEN_EOF) {
            pos++;
        }
    }
}

static int validate_input(const char *input, int max_len) {
    if (input == NULL) return 0;
    if (max_len <= 0 || max_len > MAX_INPUT_LEN) return 0;
    
    for (int i = 0; i < max_len; i++) {
        if (input[i] == '\0') break;
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_stats_t stats = {0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int input_len = (int)strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        printf("Invalid input detected\n");
        return 1;
    }
    
    process_input(input_buffer, input_len, &stats);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", stats.numbers_found);
    printf("Words found: %d\n", stats.words_found);
    printf("Errors found: %d\n", stats.errors_found);
    
    return 0;
}