//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            if (token_len > MAX_WORD_LEN) {
                fprintf(stderr, "Word too long: %s\n", token);
                return 1;
            }
            
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            
            for (int i = 0; words[word_count][i]; i++) {
                words[word_count][i] = tolower((unsigned char)words[word_count][i]);
            }
            
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count == MAX_WORDS && token != NULL) {
        fprintf(stderr, "Too many words\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWords (lowercase):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        int freq[MAX_WORDS] = {0};
        int unique_count = 0;
        
        for (int i = 0; i < word_count; i++) {
            if (freq[i] == -1) continue;
            
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (freq[j] != -1 && strcmp(words[i], words[j]) == 0) {
                    count++;
                    freq[j] = -1;
                }
            }
            freq[i] = count;
            unique_count++;
        }
        
        printf("\nWord frequency:\n");
        for (int i = 0; i < word_count; i++) {
            if (freq[i] > 0) {
                printf("%s: %d\n", words[i], freq[i]);
            }
        }
        printf("Unique words: %d\n", unique_count);
    }
    
    return 0;
}