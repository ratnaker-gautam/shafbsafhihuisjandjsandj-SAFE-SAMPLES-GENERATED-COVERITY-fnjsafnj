//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    printf("Original text: %s\n\n", input);
    
    printf("Word analysis:\n");
    for (int i = 0; i < word_count; i++) {
        int letter_count = 0;
        int digit_count = 0;
        int upper_count = 0;
        int lower_count = 0;
        
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
                if (isupper((unsigned char)words[i][j])) {
                    upper_count++;
                } else {
                    lower_count++;
                }
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
        }
        
        printf("Word %d: '%s' (length: %zu, letters: %d, digits: %d, uppercase: %d, lowercase: %d)\n",
               i + 1, words[i], strlen(words[i]), letter_count, digit_count, upper_count, lower_count);
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s' appears %d time%s\n", words[i], count, count == 1 ? "" : "s");
        processed[i] = 1;
    }
    
    printf("\nModified text (uppercase first letter): ");
    int space_needed = 0;
    for (int i = 0; i < word_count; i++) {
        if (space_needed) printf(" ");
        if (strlen(words[i]) > 0) {
            char modified[MAX_WORD_LEN];
            strncpy(modified, words[i], sizeof(modified) - 1);
            modified[sizeof(modified) - 1] = '\0';
            
            if (islower((unsigned char)modified[0])) {
                modified[0] = toupper((unsigned char)modified[0]);
            }
            printf("%s", modified);
            space_needed = 1;
        }
    }
    printf("\n");
    
    return 0;
}