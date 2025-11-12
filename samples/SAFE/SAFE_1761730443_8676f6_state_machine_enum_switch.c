//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char buffer[256];
    char number_buf[32];
    char word_buf[64];
    int num_idx = 0;
    int word_idx = 0;
    enum state current = START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    printf("Processing text: %s\n", buffer);
    printf("Found elements:\n");
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current) {
            case START:
                if (c == '\0') {
                    current = END;
                } else if (isdigit((unsigned char)c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                    current = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = c;
                    }
                    current = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current = SKIPPING_SPACE;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0' || !isdigit((unsigned char)c)) {
                    if (num_idx > 0 && num_idx < sizeof(number_buf)) {
                        number_buf[num_idx] = '\0';
                        printf("Number: %s\n", number_buf);
                        num_idx = 0;
                    }
                    
                    if (c == '\0') {
                        current = END;
                    } else if (isalpha((unsigned char)c)) {
                        if (word_idx < sizeof(word_buf) - 1) {
                            word_buf[word_idx++] = c;
                        }
                        current = READING_WORD;
                    } else if (isspace((unsigned char)c)) {
                        current = SKIPPING_SPACE;
                    }
                } else {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                }
                break;
                
            case READING_WORD:
                if (c == '\0' || !isalpha((unsigned char)c)) {
                    if (word_idx > 0 && word_idx < sizeof(word_buf)) {
                        word_buf[word_idx] = '\0';
                        printf("Word: %s\n", word_buf);
                        word_idx = 0;
                    }
                    
                    if (c == '\0') {
                        current = END;
                    } else if (isdigit((unsigned char)c)) {
                        if (num_idx < sizeof(number_buf) - 1) {
                            number_buf[num_idx++] = c;
                        }
                        current = READING_NUMBER;
                    } else if (isspace((unsigned char)c)) {
                        current = SKIPPING_SPACE;
                    }
                } else {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = c;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current = END;
                } else if (isdigit((unsigned char)c)) {
                    if (num_idx < sizeof(number_buf) - 1) {
                        number_buf[num_idx++] = c;
                    }
                    current = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (word_idx < sizeof(word_buf) - 1) {
                        word_buf[word_idx++] = c;
                    }
                    current = READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current = START;
                }
                break;
                
            case END:
                break;
        }
        
        if (current == END) {
            break;
        }
    }
    
    return 0;
}