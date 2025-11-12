//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINAL, ERROR };

enum State process_char(char c, enum State current) {
    switch(current) {
        case START:
            if (c >= '0' && c <= '9') return PROCESSING;
            if (c == '-') return PROCESSING;
            return ERROR;
        case PROCESSING:
            if (c >= '0' && c <= '9') return PROCESSING;
            if (c == '\0') return FINAL;
            return ERROR;
        case FINAL:
        case ERROR:
            return current;
    }
    return ERROR;
}

int parse_integer_recursive(const char* str, enum State state, int index, int sign, int result) {
    if (str == NULL) return 0;
    
    char current_char = str[index];
    
    if (state == ERROR) return 0;
    if (state == FINAL) return result * sign;
    
    enum State next_state = process_char(current_char, state);
    
    if (next_state == ERROR) return 0;
    
    if (next_state == PROCESSING && current_char != '-') {
        int digit = current_char - '0';
        if (result > (2147483647 - digit) / 10) return 0;
        result = result * 10 + digit;
    }
    
    if (current_char == '-') {
        if (state != START) return 0;
        sign = -1;
    }
    
    return parse_integer_recursive(str, next_state, index + 1, sign, result);
}

int string_to_int(const char* str) {
    if (str == NULL || strlen(str) == 0) return 0;
    return parse_integer_recursive(str, START, 0, 1, 0);
}

int main() {
    char input[256];
    
    printf("Enter a number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    int result = string_to_int(input);
    
    if (result == 0 && input[0] != '0') {
        printf("Invalid number format\n");
    } else {
        printf("Parsed integer: %d\n", result);
    }
    
    return 0;
}