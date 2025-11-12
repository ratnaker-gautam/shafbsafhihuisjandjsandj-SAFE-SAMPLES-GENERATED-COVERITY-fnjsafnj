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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

void process_char(parser_t *parser, char c) {
    if (!is_valid_char(c)) {
        parser->current_state = STATE_ERROR;
        return;
    }

    switch (parser->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                parser->current_state = STATE_READING_NUMBER;
                parser->buffer[parser->buf_pos++] = c;
            } else if (isalpha(c)) {
                parser->current_state = STATE_READING_WORD;
                parser->buffer[parser->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                break;
            } else {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (parser->buf_pos < MAX_INPUT_LEN) {
                    parser->buffer[parser->buf_pos++] = c;
                } else {
                    parser->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                parser->number_count++;
                parser->buf_pos = 0;
                memset(parser->buffer, 0, sizeof(parser->buffer));
                parser->current_state = STATE_START;
            } else {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (parser->buf_pos < MAX_INPUT_LEN) {
                    parser->buffer[parser->buf_pos++] = c;
                } else {
                    parser->current_state = STATE_ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                parser->word_count++;
                parser->buf_pos = 0;
                memset(parser->buffer, 0, sizeof(parser->buffer));
                parser->current_state = STATE_START;
            } else {
                parser->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
}

void finalize_parsing(parser_t *parser) {
    if (parser->current_state == STATE_READING_NUMBER) {
        parser->number_count++;
    } else if (parser->current_state == STATE_READING_WORD) {
        parser->word_count++;
    }
    parser->current_state = STATE_DONE;
}

int main(void) {
    parser_t parser;
    char input[MAX_INPUT_LEN + 1];
    int bytes_read;
    
    parser_init(&parser);
    
    printf("Enter text (numbers and words separated by spaces): ");
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
        process_char(&parser, input[i]);
        if (parser.current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    finalize_parsing(&parser);
    
    if (parser.current_state == STATE_ERROR) {
        fprintf(stderr, "Parsing error occurred\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}