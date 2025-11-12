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

static token_type_t get_next_token(const char *input, size_t *pos) {
    if (input[*pos] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isspace((unsigned char)input[*pos])) {
        (*pos)++;
        return TOKEN_SPACE;
    }
    
    if (isdigit((unsigned char)input[*pos])) {
        return TOKEN_DIGIT;
    }
    
    if (isalpha((unsigned char)input[*pos])) {
        return TOKEN_LETTER;
    }
    
    return TOKEN_INVALID;
}

static void process_state_start(token_type_t token, parser_state_t *state, size_t *pos) {
    switch (token) {
        case TOKEN_DIGIT:
            *state = STATE_READING_NUMBER;
            break;
        case TOKEN_LETTER:
            *state = STATE_READING_WORD;
            break;
        case TOKEN_SPACE:
            (*pos)++;
            break;
        case TOKEN_EOF:
            *state = STATE_END;
            break;
        default:
            *state = STATE_ERROR;
            break;
    }
}

static void process_state_reading_number(token_type_t token, parser_state_t *state, size_t *pos, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_DIGIT:
            (*pos)++;
            break;
        case TOKEN_SPACE:
            stats->numbers_found++;
            *state = STATE_START;
            (*pos)++;
            break;
        case TOKEN_EOF:
            stats->numbers_found++;
            *state = STATE_END;
            break;
        default:
            *state = STATE_ERROR;
            break;
    }
}

static void process_state_reading_word(token_type_t token, parser_state_t *state, size_t *pos, parser_stats_t *stats) {
    switch (token) {
        case TOKEN_LETTER:
            (*pos)++;
            break;
        case TOKEN_SPACE:
            stats->words_found++;
            *state = STATE_START;
            (*pos)++;
            break;
        case TOKEN_EOF:
            stats->words_found++;
            *state = STATE_END;
            break;
        default:
            *state = STATE_ERROR;
            break;
    }
}

static void process_state_error(parser_state_t *state, size_t *pos, parser_stats_t *stats) {
    stats->errors_found++;
    (*pos)++;
    *state = STATE_START;
}

static int parse_input(const char *input, parser_stats_t *stats) {
    if (input == NULL || stats == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    parser_state_t state = STATE_START;
    size_t pos = 0;
    
    stats->numbers_found = 0;
    stats->words_found = 0;
    stats->errors_found = 0;
    
    while (state != STATE_END && pos < input_len) {
        token_type_t token = get_next_token(input, &pos);
        
        switch (state) {
            case STATE_START:
                process_state_start(token, &state, &pos);
                break;
            case STATE_READING_NUMBER:
                process_state_reading_number(token, &state, &pos, stats);
                break;
            case STATE_READING_WORD:
                process_state_reading_word(token, &state, &pos, stats);
                break;
            case STATE_ERROR:
                process_state_error(&state, &pos, stats);
                break;
            case STATE_END:
                break;
        }
    }
    
    return 0;
}

static int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    printf("Enter text to parse (max %zu characters): ", buffer_size - 1);
    
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
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_stats_t stats;
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (parse_input(input_buffer, &stats) != 0) {
        fprintf(stderr, "Error parsing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Parsing results:\n");
    printf("Numbers