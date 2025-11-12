//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: parser_tokenizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct ParserState {
    const char *input;
    int position;
    int line;
    int column;
    int error_count;
};

void init_parser(struct ParserState *state, const char *input) {
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_UNKNOWN, "", state->line, state->column};
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    while (isspace((unsigned char)state->input[state->position])) {
        if (state->input[state->position] == '\n') {
            state->line++;
            state->column = 1;
        } else {
            state->column++;
        }
        state->position++;
    }
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    token.line = state->line;
    token.column = state->column;
    
    char current = state->input[state->position];
    
    if (isdigit((unsigned char)current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)current) || current == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (is_operator_char(current)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        token.value[0] = '"';
        state->position++;
        state->column++;
        int i = 1;
        while (state->input[state->position] != '"' && state->input[state->position] != '\0' && i < 30) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        if (state->input[state->position] == '"') {
            token.value[i++] = '"';
            state->position++;
            state->column++;
        }
        token.value[i] = '\0';
    } else {
        token.type = TOKEN_UNKNOWN;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "UNKNOWN"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to tokenize (max 1023 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1 && input_buffer[0] == '\n') {
        printf("No input provided\n");
        return 0;
    }
    
    struct ParserState state;
    init_parser(&state, input_buffer);
    
    printf("\nTokenized output:\n");
    printf("================\n");
    
    int token_count = 0;
    struct Token token;