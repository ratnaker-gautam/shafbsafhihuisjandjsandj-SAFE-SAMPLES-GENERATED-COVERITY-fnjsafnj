//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    int numbers[100];
    char words[100][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = INIT;
    
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
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    size_t pos = 0;
    while (pos < len && current_state != ERROR && current_state != DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case INIT:
                if (isspace(c)) {
                    pos++;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                {
                    char num_str[20];
                    int num_len = 0;
                    
                    while (pos < len && isdigit(buffer[pos]) && num_len < 19) {
                        num_str[num_len++] = buffer[pos++];
                    }
                    num_str[num_len] = '\0';
                    
                    if (num_count < 100) {
                        long val = strtol(num_str, NULL, 10);
                        if (val >= -2147483647 && val <= 2147483647) {
                            numbers[num_count++] = (int)val;
                        } else {
                            current_state = ERROR;
                        }
                    } else {
                        current_state = ERROR;
                    }
                    
                    if (pos >= len) {
                        current_state = DONE;
                    } else if (!isspace(buffer[pos])) {
                        current_state = ERROR;
                    } else {
                        current_state = INIT;
                    }
                }
                break;
                
            case READING_WORD:
                {
                    int word_len = 0;
                    
                    while (pos < len && isalpha(buffer[pos]) && word_len < 49) {
                        words[word_count][word_len++] = buffer[pos++];
                    }
                    words[word_count][word_len] = '\0';
                    
                    if (word_count < 100) {
                        word_count++;
                    } else {
                        current_state = ERROR;
                    }
                    
                    if (pos >= len) {
                        current_state = DONE;
                    } else if (!isspace(buffer[pos])) {
                        current_state = ERROR;
                    } else {
                        current_state = INIT;
                    }
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Parsed %d numbers: ", num_count);
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Parsed %d words: ", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}