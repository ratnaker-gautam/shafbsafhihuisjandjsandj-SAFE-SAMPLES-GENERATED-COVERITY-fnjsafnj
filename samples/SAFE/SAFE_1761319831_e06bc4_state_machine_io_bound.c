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

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN + 1];
    int buf_pos;
} parser_t;

void parser_init(parser_t *parser) {
    parser->current_state = STATE_START;
    parser->number_count = 0;
    parser->word_count = 0;
    parser->buf_pos = 0;
    memset(parser->buffer, 0, sizeof(parser->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || isspace(c) || c == '.' || c == '-';
}

parser_state_t handle_start(parser_t *parser, char c) {
    if (isspace(c)) {
        return STATE_START;
    } else if (isdigit(c) || c == '-' || c == '.') {
        if (parser->buf_pos < MAX_INPUT_LEN) {
            parser->buffer[parser->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        if (parser->buf_pos < MAX_INPUT_LEN) {
            parser->buffer[parser->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(parser_t *parser, char c) {
    if (isspace(c)) {
        parser->number_count++;
        parser->buf_pos = 0;
        memset(parser->buffer, 0, sizeof(parser->buffer));
        return STATE_START;
    } else if (isdigit(c) || c == '.' || c == '-') {
        if (parser->buf_pos < MAX_INPUT_LEN) {
            parser->buffer[parser->buf_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_t *parser, char c) {
    if (isspace(c)) {
        parser->word_count++;
        parser->buf_pos = 0;
        memset(parser->buffer, 0, sizeof(parser->buffer));
        return STATE_START;
    } else if (isalnum(c)) {
        if (parser->buf_pos < MAX_INPUT_LEN) {
            parser->buffer[parser->buf_pos++] = c;
        }
        return STATE_READING_WORD;
    } else {
        return STATE_ERROR;
    }
}

int process_input(parser_t *parser, const char *input) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        if (!is_valid_char(c)) {
            parser->current_state = STATE_ERROR;
            return 0;
        }
        
        switch (parser->current_state) {
            case STATE_START:
                parser->current_state = handle_start(parser, c);
                break;
            case STATE_READING_NUMBER:
                parser->current_state = handle_reading_number(parser, c);
                break;
            case STATE_READING_WORD:
                parser->current_state = handle_reading_word(parser, c);
                break;
            case STATE_ERROR:
                return 0;
            case STATE_DONE:
                return 1;
        }
        
        if (parser->current_state == STATE_ERROR) {
            return 0;
        }
    }
    
    if (parser->current_state == STATE_READING_NUMBER) {
        parser->number_count++;
    } else if (parser->current_state == STATE_READING_WORD) {
        parser->word_count++;
    }
    
    parser->current_state = STATE_DONE;
    return 1;
}

int main(void) {
    parser_t parser;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    parser_init(&parser);
    
    if (!process_input(&parser, input)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}