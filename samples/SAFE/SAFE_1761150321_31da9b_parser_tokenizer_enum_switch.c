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

struct Parser {
    const char *input;
    int position;
    int line;
    int column;
    struct Token current_token;
};

int is_operator_char(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void init_parser(struct Parser *parser, const char *input) {
    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;
    parser->current_token.type = TOKEN_ERROR;
    memset(parser->current_token.value, 0, sizeof(parser->current_token.value));
}

char peek_char(struct Parser *parser) {
    if (parser->input[parser->position] == '\0') {
        return '\0';
    }
    return parser->input[parser->position];
}

char next_char(struct Parser *parser) {
    char c = peek_char(parser);
    if (c != '\0') {
        parser->position++;
        if (c == '\n') {
            parser->line++;
            parser->column = 1;
        } else {
            parser->column++;
        }
    }
    return c;
}

void skip_whitespace(struct Parser *parser) {
    while (isspace((unsigned char)peek_char(parser))) {
        next_char(parser);
    }
}

struct Token get_next_token(struct Parser *parser) {
    struct Token token;
    memset(token.value, 0, sizeof(token.value));
    token.line = parser->line;
    token.column = parser->column;
    
    skip_whitespace(parser);
    
    char c = peek_char(parser);
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    if (isdigit((unsigned char)c)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit((unsigned char)peek_char(parser)) && i < 31) {
            token.value[i++] = next_char(parser);
        }
        token.value[i] = '\0';
    } else if (isalpha((unsigned char)c) || c == '_') {
        token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while ((isalnum((unsigned char)peek_char(parser)) || peek_char(parser) == '_') && i < 31) {
            token.value[i++] = next_char(parser);
        }
        token.value[i] = '\0';
    } else if (is_operator_char(c)) {
        token.type = TOKEN_OPERATOR;
        int i = 0;
        while (is_operator_char(peek_char(parser)) && i < 31) {
            token.value[i++] = next_char(parser);
        }
        token.value[i] = '\0';
    } else if (c == '"') {
        token.type = TOKEN_STRING;
        next_char(parser);
        int i = 0;
        while (peek_char(parser) != '"' && peek_char(parser) != '\0' && i < 31) {
            token.value[i++] = next_char(parser);
        }
        token.value[i] = '\0';
        if (peek_char(parser) == '"') {
            next_char(parser);
        }
    } else {
        token.type = TOKEN_ERROR;
        token.value[0] = next_char(parser);
        token.value[1] = '\0';
    }
    
    return token;
}

void print_token(struct Token token) {
    const char *type_names[] = {
        "NUMBER", "IDENTIFIER", "OPERATOR", "STRING", "EOF", "ERROR"
    };
    
    printf("Line %d, Column %d: %s", token.line, token.column, type_names[token.type]);
    if (token.type != TOKEN_EOF) {
        printf(" ('%s')", token.value);
    }
    printf("\n");
}

int main(void) {
    char input[1024];
    
    printf("Enter text to tokenize (max 1023 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("No input provided\n");
        return 0;
    }
    
    struct Parser parser;
    init_parser(&parser, input);
    
    printf("Tokens:\n");
    printf("-------\n");
    
    while (1) {
        struct Token token = get_next_token(&parser);
        print_token(token);
        
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR) {
            break;
        }
    }
    
    return 0;
}