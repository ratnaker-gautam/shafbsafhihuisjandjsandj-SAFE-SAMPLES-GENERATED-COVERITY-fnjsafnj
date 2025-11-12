//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct Parser {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    int buffer_pos;
};

void init_parser(struct Parser *p) {
    p->current_state = START;
    p->number_count = 0;
    p->word_count = 0;
    p->buffer_pos = 0;
    memset(p->buffer, 0, sizeof(p->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(struct Parser *p, char c) {
    if (!is_valid_char(c)) {
        p->current_state = ERROR;
        return;
    }

    switch (p->current_state) {
        case START:
            if (isdigit(c)) {
                p->current_state = READING_NUMBER;
                p->buffer[p->buffer_pos++] = c;
            } else if (isalpha(c)) {
                p->current_state = READING_WORD;
                p->buffer[p->buffer_pos++] = c;
            } else if (c == ' ' || c == '\n' || c == '\t') {
                p->current_state = START;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                if (p->buffer_pos < 255) {
                    p->buffer[p->buffer_pos++] = c;
                } else {
                    p->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\t') {
                p->number_count++;
                p->buffer_pos = 0;
                memset(p->buffer, 0, sizeof(p->buffer));
                p->current_state = START;
            } else {
                p->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (p->buffer_pos < 255) {
                    p->buffer[p->buffer_pos++] = c;
                } else {
                    p->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\t') {
                p->word_count++;
                p->buffer_pos = 0;
                memset(p->buffer, 0, sizeof(p->buffer));
                p->current_state = START;
            } else {
                p->current_state = ERROR;
            }
            break;

        case ERROR:
        case DONE:
            break;
    }
}

void finalize_parser(struct Parser *p) {
    if (p->current_state == READING_NUMBER) {
        p->number_count++;
    } else if (p->current_state == READING_WORD) {
        p->word_count++;
    }
    p->current_state = DONE;
}

int main(void) {
    struct Parser parser;
    init_parser(&parser);
    
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && parser.current_state != ERROR) {
        process_char(&parser, input[i]);
        i++;
    }
    
    if (parser.current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    finalize_parser(&parser);
    
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    
    return 0;
}