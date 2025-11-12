//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

static token_type_t get_next_token(char c) {
    if (c == '\0') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

static parser_state_t handle_start(token_type_t token, parser_stats_t *stats) {
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

static parser_state_t handle_error(token_type_t token, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static void process_input(const char *input, parser_stats_t *stats) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        stats->errors_found++;
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        token_type_t token = get_next_token(input[i]);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(token, stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(token, stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(token, stats);
                break;
            case STATE_ERROR:
                current_state = handle_error(token, stats);
                break;
            case STATE_END:
                return;
        }
    }
}

static int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    printf("Enter text to parse (max %zu chars): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_stats_t stats = {0, 0, 0};
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    process_input(input_buffer, &stats);
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", stats.numbers_found);
    printf("Words found: %d\n", stats.words_found);
    printf("Errors found: %d\n", stats.errors_found);
    
    return EXIT_SUCCESS;
}