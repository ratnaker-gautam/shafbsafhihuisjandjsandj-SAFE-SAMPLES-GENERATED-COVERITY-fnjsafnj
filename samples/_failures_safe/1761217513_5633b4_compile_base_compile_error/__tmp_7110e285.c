//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
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
    enum state current_state = STATE_START;
    
    memset(token->value, 0, sizeof(token->value));
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = get_next_char(stream);
        
        switch (current_state) {
            case STATE_START:
                if (c == EOF) {
                    token->type = TOKEN_EOF;
                    current_state = STATE_DONE;
                } else if (isdigit(c)) {
                    if (pos < sizeof(token->value) - 1) {
                        token->value[pos++] = c;
                    }
                    token->type = TOKEN_NUMBER;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    if (pos < sizeof(token->value) - 1) {
                        token->value[pos++] = c;
                    }
                    token->type = TOKEN_WORD;
                    current_state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else if (c == -1) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                    if (c != EOF) ungetc(c, stream);
                    token->value[pos] = '\0';
                    current_state = STATE_DONE;
                } else if (isdigit(c)) {
                    if (pos < sizeof(token->value) - 1) {
                        token->value[pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                    if (c != EOF) ungetc(c, stream);
                    token->value[pos] = '\0';
                    current_state = STATE_DONE;
                } else if (isalnum(c)) {
                    if (pos < sizeof(token->value) - 1) {
                        token->value[pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    return 0;
}

void process_tokens(FILE *stream) {
    struct token token;
    int token_count = 0;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Parsing input...\n");
    
    while (1) {
        if (parse_token(stream, &token) != 0) {
            printf("Error: Invalid token encountered\n");
            break;
        }
        
        if (token.type == TOKEN_EOF) {
            printf("End of input reached\n");
            break;
        }
        
        token_count++;
        
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", token.value);
                number_sum += atoi(token.value);
                break;
            case TOKEN_WORD:
                printf("Word: %s\n", token.value);
                word_count++;
                break;
            default:
                break;
        }
        
        if (token_count >= 100) {
            printf("Maximum token limit reached\n");
            break;
        }
    }
    
    printf("\nSummary:\n");
    printf("Total tokens: %d\n", token_count);
    printf("Numbers found: %d\n", token_count - word_count);
    printf("Words found: %d\n", word_count);
    printf("Sum of numbers: %d\n", number_sum);
}

int main(void) {
    char input[256];
    
    printf("Enter text (alphanumeric characters and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    FILE *stream = fmemopen(input, strlen(input), "r");
    if (stream == NULL) {
        printf("Error creating stream\n");
        return 1;
    }
    
    process_tokens(stream);
    
    fclose(stream);