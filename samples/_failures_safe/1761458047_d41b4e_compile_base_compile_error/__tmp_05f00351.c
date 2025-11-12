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
    TOKEN_INVALID
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
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    
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
        
        if (state->input[state->position] == '\0') {
            token.type = TOKEN_EOF;
            strcpy(token.value, "EOF");
            return token;
        }
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
    } else if (current == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        state->position++;
        state->column++;
        
        while (state->input[state->position] != '"' && state->input[state->position] != '\0' && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        } else {
            state->error_count++;
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
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
        state->error_count++;
    }
    
    return token;
}

void print_token(const struct Token *token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
    };
    
    printf("Line %d, Column %d: %s", token->line, token->column, type_names[token->type]);
    
    if (token->type != TOKEN_EOF) {
        printf(" ('%s')", token->value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len <= 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct ParserState state;
    init_parser(&state, input);
    
    printf("Tokens:\n");
    printf("-------\n");
    
    int token_count = 0;
    struct Token token;
    
    do {
        token = get_next_token(&state);
        print