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
    TOKEN_ERROR
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

struct Token next_token(struct ParserState *state) {
    struct Token token = {TOKEN_ERROR, "", state->line, state->column};
    
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
            token.type = TOKEN_NUMBER;
            int i = 0;
            while (isdigit(state->input[state->position]) && i < 31) {
                token.value[i++] = state->input[state->position++];
                state->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            token.type = TOKEN_IDENTIFIER;
            int i = 0;
            while ((isalnum(state->input[state->position]) || state->input[state->position] == '_') && i < 31) {
                token.value[i++] = state->input[state->position++];
                state->column++;
            }
            token.value[i] = '\0';
            return token;
        }
        
        if (is_operator_char(current)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = current;
            token.value[1] = '\0';
            state->position++;
            state->column++;
            return token;
        }
        
        if (current == '"') {
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
                token.type = TOKEN_ERROR;
            }
            token.value[i] = '\0';
            return token;
        }
        
        token.type = TOKEN_ERROR;
        strncpy(token.value, "Unexpected character", 31);
        state->position++;
        state->column++;
        return token;
    }
    
    token.type = TOKEN_EOF;
    strncpy(token.value, "EOF", 31);
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" '%s'", token.value);
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
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct ParserState state;
    init_parser(&state, input_buffer);
    
    printf("Tokens:\n");
    
    struct Token token;
    do {
        token = next_token(&state);
        print_token(token);
        
        if (token.type == TOKEN_ERROR) {
            state.error_count++;
        }
    } while (token.type != TOKEN_EOF && state.error_count < 10);
    
    if (state.error_count > 0) {
        printf("Encountered %d errors during tokenization\n", state.error_count);
        return 1;
    }
    
    return 0;
}