//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < (int)len && current_state != ERROR && current_state != DONE) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isspace((unsigned char)c)) {
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isspace((unsigned char)c)) {
                    current_state = START;
                    i++;
                } else if (isdigit((unsigned char)c)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace((unsigned char)c)) {
                    current_state = START;
                    i++;
                } else if (isalpha((unsigned char)c)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", num_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}