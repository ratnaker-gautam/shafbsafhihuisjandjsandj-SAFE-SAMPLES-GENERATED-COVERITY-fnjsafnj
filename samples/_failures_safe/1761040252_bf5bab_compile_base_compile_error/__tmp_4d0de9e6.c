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
} parser_result_t;

static token_type_t get_next_token(const char *input, size_t *pos) {
    if (input[*pos] == '\0') {
        return TOKEN_EOF;
    }
    
    if (isspace((unsigned char)input[*pos])) {
        (*pos)++;
        return TOKEN_SPACE;
    }
    
    if (isdigit((unsigned char)input[*pos])) {
        (*pos)++;
        return TOKEN_DIGIT;
    }
    
    if (isalpha((unsigned char)input[*pos])) {
        (*pos)++;
        return TOKEN_LETTER;
    }
    
    (*pos)++;
    return TOKEN_INVALID;
}

static void initialize_result(parser_result_t *result) {
    result->numbers_found = 0;
    result->words_found = 0;
    result->errors_found = 0;
}

static parser_state_t handle_start_state(token_type_t token, parser_result_t *result) {
    switch (token) {
        case TOKEN_DIGIT:
            result->numbers_found++;
            return STATE_READING_NUMBER;
        case TOKEN_LETTER:
            result->words_found++;
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            result->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_reading_number(token_type_t token, parser_result_t *result) {
    switch (token) {
        case TOKEN_DIGIT:
            return STATE_READING_NUMBER;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            result->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_reading_word(token_type_t token, parser_result_t *result) {
    switch (token) {
        case TOKEN_LETTER:
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            result->errors_found++;
            return STATE_ERROR;
    }
}

static parser_state_t handle_error_state(token_type_t token) {
    switch (token) {
        case TOKEN_SPACE:
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static int parse_input(const char *input, parser_result_t *result) {
    if (input == NULL || result == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    initialize_result(result);
    
    parser_state_t current_state = STATE_START;
    size_t position = 0;
    
    while (current_state != STATE_END) {
        token_type_t token = get_next_token(input, &position);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start_state(token, result);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(token, result);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(token, result);
                break;
            case STATE_ERROR:
                current_state = handle_error_state(token);
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
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_result_t result;
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (parse_input(input_buffer, &result) != 0) {
        fprintf(stderr, "Error parsing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", result.numbers_found);
    printf("Words found: %d\n", result.words_found);
    printf("Errors found: %d\n", result.errors_found);
    
    return