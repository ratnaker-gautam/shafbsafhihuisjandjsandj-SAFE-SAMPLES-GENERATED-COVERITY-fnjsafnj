//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, SKIPPING, DONE };

int main(void) {
    char input[1024];
    char buffer[1024];
    int pos = 0;
    enum State state = START;
    int num_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < (int)len && state != DONE) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isdigit(c)) {
                    state = READING_NUM;
                    buffer[0] = c;
                    pos = 1;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                    buffer[0] = c;
                    pos = 1;
                } else {
                    state = SKIPPING;
                }
                break;
                
            case READING_NUM:
                if (isdigit(c)) {
                    if (pos < (int)sizeof(buffer) - 1) {
                        buffer[pos++] = c;
                    } else {
                        state = SKIPPING;
                    }
                } else {
                    buffer[pos] = '\0';
                    printf("Found number: %s\n", buffer);
                    num_count++;
                    state = isalpha(c) ? READING_WORD : SKIPPING;
                    if (state == READING_WORD) {
                        buffer[0] = c;
                        pos = 1;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (pos < (int)sizeof(buffer) - 1) {
                        buffer[pos++] = c;
                    } else {
                        state = SKIPPING;
                    }
                } else {
                    buffer[pos] = '\0';
                    printf("Found word: %s\n", buffer);
                    word_count++;
                    state = isdigit(c) ? READING_NUM : SKIPPING;
                    if (state == READING_NUM) {
                        buffer[0] = c;
                        pos = 1;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit(c)) {
                    state = READING_NUM;
                    buffer[0] = c;
                    pos = 1;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                    buffer[0] = c;
                    pos = 1;
                }
                break;
                
            case DONE:
                break;
        }
        
        i++;
    }
    
    if (state == READING_NUM || state == READING_WORD) {
        buffer[pos] = '\0';
        if (state == READING_NUM) {
            printf("Found number: %s\n", buffer);
            num_count++;
        } else {
            printf("Found word: %s\n", buffer);
            word_count++;
        }
    }
    
    printf("Summary: %d numbers, %d words found\n", num_count, word_count);
    
    return 0;
}