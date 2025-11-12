//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_INTEGER,
    READING_FLOAT,
    READING_STRING,
    ERROR,
    END
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int has_decimal = 0;
    int has_digits = 0;
    int ch;
    
    printf("Enter input (Ctrl+D to end): ");
    
    while (current_state != END && current_state != ERROR) {
        ch = getchar();
        
        switch (current_state) {
            case START:
                if (ch == EOF) {
                    current_state = END;
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        has_digits = 1;
                        current_state = READING_INTEGER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        current_state = READING_STRING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (!isspace(ch) && ch != EOF) {
                    current_state = ERROR;
                }
                break;
                
            case READING_INTEGER:
                if (ch == EOF) {
                    buffer[buffer_index] = '\0';
                    printf("Found integer: %s\n", buffer);
                    buffer_index = 0;
                    has_digits = 0;
                    current_state = END;
                } else if (ch == '.') {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        has_decimal = 1;
                        current_state = READING_FLOAT;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    printf("Found integer: %s\n", buffer);
                    buffer_index = 0;
                    has_digits = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_FLOAT:
                if (ch == EOF) {
                    buffer[buffer_index] = '\0';
                    printf("Found float: %s\n", buffer);
                    buffer_index = 0;
                    has_decimal = 0;
                    has_digits = 0;
                    current_state = END;
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        has_digits = 1;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    if (has_digits) {
                        buffer[buffer_index] = '\0';
                        printf("Found float: %s\n", buffer);
                    } else {
                        current_state = ERROR;
                    }
                    buffer_index = 0;
                    has_decimal = 0;
                    has_digits = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_STRING:
                if (ch == EOF) {
                    buffer[buffer_index] = '\0';
                    printf("Found string: %s\n", buffer);
                    buffer_index = 0;
                    current_state = END;
                } else if (isalnum(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    printf("Found string: %s\n", buffer);
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                while ((ch = getchar()) != EOF && !isspace(ch)) {
                    continue;
                }
                buffer_index = 0;
                has_decimal = 0;
                has_digits = 0;
                if (ch == EOF) {
                    current_state = END;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
    }
    
    if (current_state == READING_INTEGER && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        printf("Found integer: %s\n", buffer);
    } else if (current_state == READING_FLOAT && buffer_index > 0 && has_digits) {
        buffer[buffer_index] = '\0';
        printf("Found float: %s\n", buffer);
    } else if (current_state == READING_STRING && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        printf("Found string: %s\n", buffer);
    }
    
    return 0;
}