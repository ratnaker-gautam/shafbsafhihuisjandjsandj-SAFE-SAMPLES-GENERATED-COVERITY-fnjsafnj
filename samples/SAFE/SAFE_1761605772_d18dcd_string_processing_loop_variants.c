//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found (%d):\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("%d: ", j + 1);
        
        int k = 0;
        while (words[j][k] != '\0') {
            putchar(words[j][k]);
            k++;
        }
        putchar('\n');
    }
    
    printf("\nProcessing variants:\n");
    
    int j = 0;
    do {
        if (j >= word_count) break;
        
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        int k = 0;
        while (words[j][k] != '\0') {
            if (isupper(words[j][k])) has_upper = 1;
            if (islower(words[j][k])) has_lower = 1;
            if (isdigit(words[j][k])) has_digit = 1;
            k++;
        }
        
        printf("Word %d: ", j + 1);
        if (has_upper && has_lower) printf("mixed_case");
        else if (has_upper) printf("UPPERCASE");
        else if (has_lower) printf("lowercase");
        else printf("other");
        
        if (has_digit) printf("+digits");
        printf("\n");
        
        j++;
    } while (j < word_count);
    
    printf("\nReversed words:\n");
    for (int j = 0; j < word_count; j++) {
        printf("%d: ", j + 1);
        
        int len = strlen(words[j]);
        int k = len - 1;
        while (k >= 0) {
            putchar(words[j][k]);
            k--;
        }
        putchar('\n');
    }
    
    return 0;
}