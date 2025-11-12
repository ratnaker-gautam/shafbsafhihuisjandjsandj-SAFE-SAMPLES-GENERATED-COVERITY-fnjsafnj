//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_END,
    STATE_ERROR
};

enum token_type {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_EOF
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_char(FILE *stream) {
    int c = fgetc(stream);
    if (c == EOF) return EOF;
    if (!is_valid_char(c)) return -1;
    return c;
}

int parse_token(FILE *stream, struct token *token) {
    int c;
    int pos = 0;
    memset(token->value, 0, sizeof(token->value));
    
    while ((c = get_next_char(stream)) == ' ' || c == '\t' || c == '\n') {
        if (c == -1) return -1;
    }
    
    if (c == -1) return -1;
    if (c == EOF) {
        token->type = TOKEN_EOF;
        return 0;
    }
    
    if (isdigit(c)) {
        token->type = TOKEN_NUMBER;
        while (isdigit(c) && pos < 31) {
            token->value[pos++] = c;
            c = get_next_char(stream);
            if (c == -1) return -1;
        }
        if (c != EOF && c != ' ' && c != '\t' && c != '\n') {
            ungetc(c, stream);
        }
    } else if (isalpha(c)) {
        token->type = TOKEN_WORD;
        while (isalpha(c) && pos < 31) {
            token->value[pos++] = tolower(c);
            c = get_next_char(stream);
            if (c == -1) return -1;
        }
        if (c != EOF && c != ' ' && c != '\t' && c != '\n') {
            ungetc(c, stream);
        }
    } else {
        return -1;
    }
    
    token->value[31] = '\0';
    return 0;
}

void process_state_machine(FILE *stream) {
    enum state current_state = STATE_START;
    struct token token;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (parse_token(stream, &token) == -1) {
                    current_state = STATE_ERROR;
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_READING:
                switch (token.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_NUMBER;
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_WORD;
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_NUMBER:
                number_count++;
                printf("Found number: %s\n", token.value);
                if (parse_token(stream, &token) == -1) {
                    current_state = STATE_ERROR;
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_WORD:
                word_count++;
                printf("Found word: %s\n", token.value);
                if (parse_token(stream, &token) == -1) {
                    current_state = STATE_ERROR;
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_END:
            case STATE_ERROR:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    }
}

int main(void) {
    char input[256];
    
    printf("Enter text (alphanumeric characters only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 1 && input[0] == '\0') {
        printf("No input provided\n");
        return 1;
    }
    
    FILE *stream = fmemopen(input, strlen(input), "r");
    if (stream == NULL) {
        printf("Error creating stream\n");
        return 1;
    }
    
    process_state_machine(stream);
    fclose(stream);
    
    return 0;
}