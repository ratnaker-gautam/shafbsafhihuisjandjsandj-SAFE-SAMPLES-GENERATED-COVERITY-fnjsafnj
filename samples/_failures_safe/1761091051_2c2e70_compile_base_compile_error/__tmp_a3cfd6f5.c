//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LINE 1024
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

enum token_type {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN
};

struct token {
    enum token_type type;
    char value[MAX_TOKEN_LEN];
};

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

int is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' || c == ';' || c == '.';
}

int parse_number(const char *str, int start, int len, char *result) {
    int i = start;
    int has_dot = 0;
    int result_len = 0;
    
    while (i < len && result_len < MAX_TOKEN_LEN - 1) {
        if (isdigit(str[i])) {
            result[result_len++] = str[i];
        } else if (str[i] == '.' && !has_dot) {
            result[result_len++] = str[i];
            has_dot = 1;
        } else {
            break;
        }
        i++;
    }
    
    if (result_len > 0) {
        result[result_len] = '\0';
        return i - start;
    }
    return 0;
}

int parse_word(const char *str, int start, int len, char *result) {
    int i = start;
    int result_len = 0;
    
    while (i < len && result_len < MAX_TOKEN_LEN - 1) {
        if (isalpha(str[i]) || str[i] == '_' || (i > start && isdigit(str[i]))) {
            result[result_len++] = str[i];
        } else {
            break;
        }
        i++;
    }
    
    if (result_len > 0) {
        result[result_len] = '\0';
        return i - start;
    }
    return 0;
}

int tokenize_line(const char *line, struct token *tokens, int *token_count) {
    int len = strlen(line);
    int pos = 0;
    int count = 0;
    
    while (pos < len && count < MAX_TOKENS - 1) {
        while (pos < len && isspace(line[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        char buffer[MAX_TOKEN_LEN];
        int consumed = 0;
        
        if (isdigit(line[pos])) {
            consumed = parse_number(line, pos, len, buffer);
            if (consumed > 0) {
                tokens[count].type = TOKEN_NUMBER;
                strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
                count++;
                pos += consumed;
                continue;
            }
        }
        
        if (isalpha(line[pos]) || line[pos] == '_') {
            consumed = parse_word(line, pos, len, buffer);
            if (consumed > 0) {
                tokens[count].type = TOKEN_WORD;
                strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
                tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
                count++;
                pos += consumed;
                continue;
            }
        }
        
        if (is_operator(line[pos])) {
            buffer[0] = line[pos];
            buffer[1] = '\0';
            tokens[count].type = TOKEN_OPERATOR;
            strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
            count++;
            pos++;
            continue;
        }
        
        if (is_punctuation(line[pos])) {
            buffer[0] = line[pos];
            buffer[1] = '\0';
            tokens[count].type = TOKEN_PUNCTUATION;
            strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
            tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
            count++;
            pos++;
            continue;
        }
        
        buffer[0] = line[pos];
        buffer[1] = '\0';
        tokens[count].type = TOKEN_UNKNOWN;
        strncpy(tokens[count].value, buffer, MAX_TOKEN_LEN - 1);
        tokens[count].value[MAX_TOKEN_LEN - 1] = '\0';
        count++;
        pos++;
    }
    
    *token_count = count;
    return 0;
}

const char *token_type_name(enum token_type type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_WORD: return "WORD";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

int main(void) {