//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[1024];
    char buffer[1024];
    int buffer_pos = 0;
    enum State state = START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (c == '\0') {
                    state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buffer_pos++] = c;
                    state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer[buffer_pos++] = c;
                    state = READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else if (isdigit((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    state = (c == '\0') ? DONE : START;
                } else if (isalpha((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        state = ERROR;
                    }
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}