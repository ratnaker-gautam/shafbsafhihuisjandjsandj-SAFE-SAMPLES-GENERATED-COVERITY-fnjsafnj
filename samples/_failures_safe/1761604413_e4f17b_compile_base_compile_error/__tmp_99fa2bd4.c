//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

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
    int current_number;
    char current_word[MAX_INPUT_LEN];
} parser_data_t;

static token_type_t get_next_token(const char *input, int *pos) {
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

static void reset_parser_data(parser_data_t *data) {
    data->numbers_found = 0;
    data->words_found = 0;
    data->current_number = 0;
    memset(data->current_word, 0, sizeof(data->current_word));
}

static int handle_start_state(token_type_t token, parser_data_t *data, const char *input, int *pos) {
    switch (token) {
        case TOKEN_DIGIT:
            data->current_number = input[*pos] - '0';
            (*pos)++;
            return STATE_READING_NUMBER;
        case TOKEN_LETTER:
            data->current_word[0] = input[*pos];
            data->current_word[1] = '\0';
            (*pos)++;
            return STATE_READING_WORD;
        case TOKEN_SPACE:
            (*pos)++;
            return STATE_START;
        case TOKEN_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static int handle_reading_number(token_type_t token, parser_data_t *data, const char *input, int *pos) {
    switch (token) {
        case TOKEN_DIGIT:
            if (data->current_number <= (INT_MAX - (input[*pos] - '0')) / 10) {
                data->current_number = data->current_number * 10 + (input[*pos] - '0');
                (*pos)++;
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case TOKEN_SPACE:
            data->numbers_found++;
            (*pos)++;
            return STATE_START;
        case TOKEN_EOF:
            data->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static int handle_reading_word(token_type_t token, parser_data_t *data, const char *input, int *pos) {
    size_t current_len = strlen(data->current_word);
    
    switch (token) {
        case TOKEN_LETTER:
            if (current_len < MAX_INPUT_LEN - 1) {
                data->current_word[current_len] = input[*pos];
                data->current_word[current_len + 1] = '\0';
                (*pos)++;
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case TOKEN_SPACE:
            data->words_found++;
            (*pos)++;
            return STATE_START;
        case TOKEN_EOF:
            data->words_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static int process_input(const char *input, parser_data_t *data) {
    int pos = 0;
    parser_state_t current_state = STATE_START;
    
    reset_parser_data(data);
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        token_type_t token = get_next_token(input, &pos);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start_state(token, data, input, &pos);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(token, data, input, &pos);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(token, data, input, &pos);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    return current_state == STATE_END ? 0 : -1;
}

static int read_input_safely(char *buffer, size_t size) {
    if (size == 0 || buffer == NULL) {
        return -1;
    }
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2