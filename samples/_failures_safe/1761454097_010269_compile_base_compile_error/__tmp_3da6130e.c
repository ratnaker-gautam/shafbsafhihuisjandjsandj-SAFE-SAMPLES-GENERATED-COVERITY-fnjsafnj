//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: parser_tokenizer
#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
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

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')';
}

void skip_whitespace(struct ParserState *state) {
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

struct Token get_next_token(struct ParserState *state) {
    struct Token token = {TOKEN_INVALID, "", state->line, state->column};
    
    skip_whitespace(state);
    
    if (state->input[state->position] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "");
        return token;
    }
    
    char current = state->input[state->position];
    
    if (isdigit(current)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(state->input[state->position]) && i < 31) {
            token.value[i++] = state->input[state->position++];
            state->column++;
        }
        token.value[i] = '\0';
    } else if (isalpha(current)) {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
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
    } else {
        token.type = TOKEN_INVALID;
        token.value[0] = state->input[state->position++];
        token.value[1] = '\0';
        state->column++;
        state->error_count++;
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "EOF", "INVALID"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
    }
    printf("\n");
}

void parse_expression(struct ParserState *state);

void parse_primary(struct ParserState *state) {
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        print_token(token);
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        printf("Line %d, Column %d: GROUP_START\n", token.line, token.column);
        parse_expression(state);
        token = get_next_token(state);
        if (token.type == TOKEN_OPERATOR && token.value[0] == ')') {
            printf("Line %d, Column %d: GROUP_END\n", token.line, token.column);
        } else {
            printf("Line %d, Column %d: ERROR Expected ')'\n", token.line, token.column);
            state->error_count++;
        }
    } else {
        printf("Line %d, Column %d: ERROR Unexpected token\n", token.line, token.column);
        state->error_count++;
    }
}

void parse_expression(struct ParserState *state) {
    parse_primary(state);
    
    struct Token token = get_next_token(state);
    
    if (token.type == TOKEN_OPERATOR && 
        (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0 || 
         strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0)) {
        print_token(token);
        parse_expression(state);
    } else {
        state->position -= strlen(token.value);
        state->column -= strlen(token.value);
    }
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input