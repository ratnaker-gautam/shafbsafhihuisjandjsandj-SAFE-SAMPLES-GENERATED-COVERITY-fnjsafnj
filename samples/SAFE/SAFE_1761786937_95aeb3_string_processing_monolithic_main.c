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
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            if (token_len <= MAX_WORD_LEN) {
                strncpy(words[word_count], token, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            } else {
                fprintf(stderr, "Word too long: %s\n", token);
                return 1;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count >= MAX_WORDS && token != NULL) {
        fprintf(stderr, "Too many words\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord list:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %zu\n", words[i], strlen(words[i]));
        }
        
        int unique_count = 0;
        printf("\nUnique words:\n");
        for (int i = 0; i < word_count; i++) {
            int is_unique = 1;
            for (int j = 0; j < i; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) {
                printf("%s\n", words[i]);
                unique_count++;
            }
        }
        printf("Total unique words: %d\n", unique_count);
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    int rev_index = 0;
    for (int i = char_count - 1; i >= 0; i--) {
        if (rev_index < MAX_INPUT_LEN) {
            reversed[rev_index++] = input[i];
        }
    }
    reversed[rev_index] = '\0';
    
    printf("\nReversed text:\n%s\n", reversed);
    
    char processed[MAX_INPUT_LEN + 1];
    int proc_index = 0;
    for (int i = 0; i < char_count && proc_index < MAX_INPUT_LEN; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            if (isupper((unsigned char)c)) {
                processed[proc_index++] = tolower((unsigned char)c);
            } else {
                processed[proc_index++] = toupper((unsigned char)c);
            }
        } else {
            processed[proc_index++] = c;
        }
    }
    processed[proc_index] = '\0';
    
    printf("\nCase-swapped text:\n%s\n", processed);
    
    return 0;
}