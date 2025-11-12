//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: parser_tokenizer
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
    
    while (state->input[state->position] != '\0') {
        char current = state->input[state->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                state->line++;
                state->column = 1;
            } else {
                state->column++;
            }
            state->position++;
            continue;
        }
        
        if (isdigit(current)) {
            int i = 0;
            while (state->input[state->position] != '\0' && 
                   i < 31 && 
                   isdigit(state->input[state->position])) {
                token.value[i++] = state->input[state->position++];
                state->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while (state->input[state->position] != '\0' && 
                   i < 31 && 
                   (isalnum(state->input[state->position]) || 
                    state->input[state->position] == '_')) {
                token.value[i++] = state->input[state->position++];
                state->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (current == '"') {
            int i = 0;
            state->position++;
            state->column++;
            
            while (state->input[state->position] != '\0' && 
                   i < 31 && 
                   state->input[state->position] != '"') {
                token.value[i++] = state->input[state->position++];
                state->column++;
            }
            
            if (state->input[state->position] == '"') {
                state->position++;
                state->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
            return token;
        }
        
        if (is_operator_char(current)) {
            token.value[0] = current;
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
            state->position++;
            state->column++;
            return token;
        }
        
        state->error_count++;
        state->position++;
        state->column++;
        token.type = TOKEN_INVALID;
        return token;
    }
    
    token.type = TOKEN_EOF;
    return token;
}

void parse_expression(struct ParserState *state) {
    struct Token tokens[100];
    int token_count = 0;
    
    while (token_count < 100) {
        struct Token token = get_next_token(state);
        tokens[token_count] = token;
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        if (token.type == TOKEN_INVALID) {
            printf("Invalid token at line %d, column %d\n", token.line, token.column);
            continue;
        }
        
        token_count++;
    }
    
    printf("Parsed %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        const char *type_names[] = {
            "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "INVALID"
        };
        printf("  %s: '%s' (line %d, col %d)\n", 
               type_names[tokens[i].type], 
               tokens[i].value, 
               tokens[i].line, 
               tokens[i].column);
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter expression to parse: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct ParserState state;
    init_parser(&state, input_buffer);
    parse_expression(&state);
    
    if (state.error