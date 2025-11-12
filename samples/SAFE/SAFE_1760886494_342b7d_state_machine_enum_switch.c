//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_INTEGER,
    READING_FLOAT,
    READING_STRING,
    END,
    ERROR
};

int main(void) {
    char input[256];
    char buffer[256];
    int buf_index = 0;
    enum state current_state = START;
    int integer_value = 0;
    double float_value = 0.0;
    int has_decimal = 0;
    int decimal_places = 0;
    
    printf("Enter data (mix of integers, floats, and strings): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (c == '\n' || c == '\0') {
            current_state = END;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isspace((unsigned char)c)) {
                    continue;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_INTEGER;
                    integer_value = c - '0';
                    buffer[0] = c;
                    buf_index = 1;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_STRING;
                    buffer[0] = c;
                    buf_index = 1;
                } else if (c == '.' || c == '+' || c == '-') {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_INTEGER:
                if (isdigit((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                        integer_value = integer_value * 10 + (c - '0');
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == '.') {
                    current_state = READING_FLOAT;
                    float_value = (double)integer_value;
                    has_decimal = 1;
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    printf("Integer: %d\n", integer_value);
                    current_state = START;
                    integer_value = 0;
                    buf_index = 0;
                } else if (isalpha((unsigned char)c)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_FLOAT:
                if (isdigit((unsigned char)c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                        decimal_places++;
                        double digit_value = (c - '0');
                        for (int j = 0; j < decimal_places; j++) {
                            digit_value /= 10.0;
                        }
                        float_value += digit_value;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    printf("Float: %.6f\n", float_value);
                    current_state = START;
                    float_value = 0.0;
                    has_decimal = 0;
                    decimal_places = 0;
                    buf_index = 0;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_STRING:
                if (isalnum((unsigned char)c) || c == '_') {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    buffer[buf_index] = '\0';
                    printf("String: %s\n", buffer);
                    current_state = START;
                    buf_index = 0;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                if (isspace((unsigned char)c)) {
                    fprintf(stderr, "Invalid token: %.*s\n", buf_index, buffer);
                    current_state = START;
                    buf_index = 0;
                    integer_value = 0;
                    float_value = 0.0;
                    has_decimal = 0;
                    decimal_places = 0;
                } else if (buf_index < sizeof(buffer) - 1) {
                    buffer[buf_index++] = c;
                }
                break;
                
            case END:
                break;
        }
        
        if (current_state == ERROR && i == input_len - 1) {
            fprintf(stderr, "Invalid token: %.*s\n", buf_index, buffer);
        }
    }
    
    if (current_state == READING_INTEGER) {
        printf("Integer: %d\n", integer_value);
    } else if (current_state == READING_FLOAT) {
        printf("Float: %.6f\n", float_value);
    } else if (current_state == READING_STRING) {
        buffer[buf_index] = '\0';
        printf("String: %s\n", buffer);
    }
    
    return 0;
}