//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_count < MAX_WORDS) {
            if (word_len > MAX_INPUT) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            for (int j = 0; j < word_len; j++) {
                words[word_count][j] = input[start + j];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        int count = 0;
        char current_word[MAX_INPUT + 1];
        strncpy(current_word, words[idx], sizeof(current_word) - 1);
        current_word[sizeof(current_word) - 1] = '\0';
        
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j], current_word) == 0) {
                count++;
            }
        }
        
        int duplicate = 0;
        for (int k = 0; k < idx; k++) {
            if (strcmp(words[k], current_word) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) {
            printf("'%s' appears %d time(s)\n", current_word, count);
        }
    }
    
    printf("Words in reverse order:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        printf("%s\n", words[idx]);
    }
    
    return 0;
}