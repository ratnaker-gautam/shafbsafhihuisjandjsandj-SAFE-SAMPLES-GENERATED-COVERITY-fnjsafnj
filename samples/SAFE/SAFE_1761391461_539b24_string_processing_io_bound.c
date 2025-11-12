//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: %s", i + 1, words[i]);
        
        size_t word_len = strlen(words[i]);
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (j = 0; j < word_len; j++) {
            if (isupper(words[i][j])) {
                has_upper = 1;
            } else if (islower(words[i][j])) {
                has_lower = 1;
            } else if (isdigit(words[i][j])) {
                has_digit = 1;
            }
        }
        
        printf(" (Length: %zu", word_len);
        if (has_upper) printf(", has uppercase");
        if (has_lower) printf(", has lowercase");
        if (has_digit) printf(", has digits");
        printf(")\n");
    }
    
    printf("\nReversed words:\n");
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        printf("Word %d reversed: ", i + 1);
        
        for (j = word_len - 1; j >= 0; j--) {
            putchar(words[i][j]);
        }
        printf("\n");
    }
    
    printf("\nWord frequencies:\n");
    for (i = 0; i < word_count; i++) {
        int count = 1;
        int counted = 0;
        
        for (k = 0; k < i; k++) {
            if (strcmp(words[i], words[k]) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (counted) {
            continue;
        }
        
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
            }
        }
        
        printf("'%s' appears %d time(s)\n", words[i], count);
    }
    
    return 0;
}