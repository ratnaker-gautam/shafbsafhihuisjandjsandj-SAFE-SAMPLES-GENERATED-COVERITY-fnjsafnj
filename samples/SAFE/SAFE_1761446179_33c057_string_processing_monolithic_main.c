//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
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
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (i = word_count - 1; i >= 0; i--) {
        printf("%s", words[i]);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    
    printf("\nWord statistics:\n");
    for (i = 0; i < word_count; i++) {
        int letter_count = 0;
        int digit_count = 0;
        
        for (j = 0; words[i][j] != '\0'; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
        }
        
        printf("Word %d: \"%s\" (letters: %d, digits: %d)\n", 
               i + 1, words[i], letter_count, digit_count);
    }
    
    printf("\nDuplicate words:\n");
    int found_duplicates = 0;
    for (i = 0; i < word_count; i++) {
        int duplicate = 0;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (duplicate) {
            printf("\"%s\" appears multiple times\n", words[i]);
            found_duplicates = 1;
        }
    }
    
    if (!found_duplicates) {
        printf("No duplicate words found\n");
    }
    
    return 0;
}