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
    TOKEN_ERROR
};

struct Token {
    enum TokenType type;
    char value[32];
    int line;
    int column;
};

struct Lexer {
    const char *input;
    int position;
    int line;
    int column;
};

void lexer_init(struct Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || 
           c == '=' || c == '<' || c == '>' || c == '!';
}

struct Token lexer_next(struct Lexer *lexer) {
    struct Token token = {TOKEN_ERROR, "", lexer->line, lexer->column};
    
    while (lexer->input[lexer->position] != '\0') {
        char current = lexer->input[lexer->position];
        
        if (isspace(current)) {
            if (current == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->position++;
            continue;
        }
        
        if (isdigit(current)) {
            int i = 0;
            while (isdigit(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        if (isalpha(current) || current == '_') {
            int i = 0;
            while ((isalnum(lexer->input[lexer->position]) || 
                   lexer->input[lexer->position] == '_') && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_IDENTIFIER;
            return token;
        }
        
        if (is_operator_char(current)) {
            int i = 0;
            while (is_operator_char(lexer->input[lexer->position]) && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            token.value[i] = '\0';
            token.type = TOKEN_OPERATOR;
            return token;
        }
        
        if (current == '"') {
            int i = 0;
            token.value[i++] = lexer->input[lexer->position++];
            lexer->column++;
            
            while (lexer->input[lexer->position] != '"' && 
                   lexer->input[lexer->position] != '\0' && i < 31) {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
            }
            
            if (lexer->input[lexer->position] == '"') {
                token.value[i++] = lexer->input[lexer->position++];
                lexer->column++;
                token.value[i] = '\0';
                token.type = TOKEN_STRING;
                return token;
            } else {
                token.type = TOKEN_ERROR;
                return token;
            }
        }
        
        token.value[0] = current;
        token.value[1] = '\0';
        lexer->position++;
        lexer->column++;
        token.type = TOKEN_ERROR;
        return token;
    }
    
    token.type = TOKEN_EOF;
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    
    if (token.type != TOKEN_EOF && token.type != TOKEN_ERROR) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Lexer lexer;
    lexer_init(&lexer, input);
    
    printf("Tokens:\n");
    
    while (1) {
        struct Token token = lexer_next(&lexer);
        print_token(token);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}