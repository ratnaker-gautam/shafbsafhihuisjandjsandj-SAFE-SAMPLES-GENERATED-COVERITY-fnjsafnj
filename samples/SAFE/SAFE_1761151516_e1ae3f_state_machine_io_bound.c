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
    char buffer[MAX_INPUT_LEN];
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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(parser_t *parser, char c) {
    if (!is_valid_char(c)) {
        parser->current_state = STATE_ERROR;
        return;
    }

    switch (parser->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                parser->buffer[parser->buf_pos++] = c;
                parser->current_state = STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                parser->buffer[parser->buf_pos++] = c;
                parser->current_state = STATE_READING_WORD;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                break;
            } else {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (parser->buf_pos < MAX_INPUT_LEN - 1) {
                    parser->buffer[parser->buf_pos++] = c;
                } else {
                    parser->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                parser->buffer[parser->buf_pos] = '\0';
                parser->number_count++;
                parser->buf_pos = 0;
                parser->current_state = STATE_START;
            } else if (isalpha(c)) {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (parser->buf_pos < MAX_INPUT_LEN - 1) {
                    parser->buffer[parser->buf_pos++] = c;
                } else {
                    parser->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                parser->buffer[parser->buf_pos] = '\0';
                parser->word_count++;
                parser->buf_pos = 0;
                parser->current_state = STATE_START;
            } else if (isdigit(c)) {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
}

int main(void) {
    parser_t parser;
    parser_init(&parser);
    
    printf("Enter text (numbers and words separated by spaces):\n");
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        process_char(&parser, input[i]);
        if (parser.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (parser.current_state == STATE_READING_NUMBER || parser.current_state == STATE_READING_WORD) {
        if (parser.buf_pos > 0) {
            if (parser.current_state == STATE_READING_NUMBER) {
                parser.number_count++;
            } else {
                parser.word_count++;
            }
        }
        parser.current_state = STATE_DONE;
    }
    
    if (parser.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}