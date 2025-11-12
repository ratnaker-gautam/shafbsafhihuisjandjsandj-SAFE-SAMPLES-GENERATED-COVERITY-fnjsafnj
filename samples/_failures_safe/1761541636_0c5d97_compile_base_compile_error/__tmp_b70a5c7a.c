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
    if (state == NULL || input == NULL) return;
    state->input = input;
    state->position = 0;
    state->line = 1;
    state->column = 1;
    state->error_count = 0;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void skip_whitespace(struct ParserState *state) {
    if (state == NULL || state->input == NULL) return;
    
    while (state->input[state->position] != '\0') {
        char c = state->input[state->position];
        if (c == ' ' || c == '\t') {
            state->position++;
            state->column++;
        } else if (c == '\n') {
            state->position++;
            state->line++;
            state->column = 1;
        } else {
            break;
        }
    }
}

int get_next_token(struct ParserState *state, struct Token *token) {
    if (state == NULL || token == NULL || state->input == NULL) return -1;
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        token->line = state->line;
        token->column = state->column;
        return 0;
    }
    
    char current = state->input[state->position];
    token->line = state->line;
    token->column = state->column;
    
    if (isdigit(current)) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (i < 31 && isdigit(state->input[state->position])) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (isalpha(current) || current == '_') {
        token->type = TOKEN_IDENTIFIER;
        int i = 0;
        while (i < 31 && (isalnum(state->input[state->position]) || state->input[state->position] == '_')) {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        token->value[i] = '\0';
    } else if (is_operator_char(current)) {
        token->type = TOKEN_OPERATOR;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
    } else if (current == '"') {
        token->type = TOKEN_STRING;
        int i = 0;
        state->position++;
        state->column++;
        
        while (i < 31 && state->input[state->position] != '"' && state->input[state->position] != '\0') {
            token->value[i++] = state->input[state->position++];
            state->column++;
        }
        
        if (state->input[state->position] == '"') {
            state->position++;
            state->column++;
        } else {
            state->error_count++;
        }
        token->value[i] = '\0';
    } else {
        token->type = TOKEN_INVALID;
        token->value[0] = state->input[state->position++];
        token->value[1] = '\0';
        state->column++;
        state->error_count++;
    }
    
    return 0;
}

const char *token_type_to_string(enum TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct ParserState state;
    struct Token token;
    
    init_parser(&state, input_buffer);
    
    printf("Tokens:\