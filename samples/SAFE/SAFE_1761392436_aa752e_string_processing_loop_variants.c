//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len < 64) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = input[i + k];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (j = 0; words[i][j] != '\0'; j++) {
            unsigned char c = words[i][j];
            if (isupper(c)) has_upper = 1;
            if (islower(c)) has_lower = 1;
            if (isdigit(c)) has_digit = 1;
        }
        
        printf("Word %d: \"%s\" (", i + 1, words[i]);
        
        if (has_upper && has_lower) {
            printf("mixed case");
        } else if (has_upper) {
            printf("all uppercase");
        } else if (has_lower) {
            printf("all lowercase");
        } else {
            printf("no letters");
        }
        
        if (has_digit) {
            printf(" + digits");
        }
        
        printf(")\n");
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        for (j = 0; words[i][j] != '\0'; j++) {
            total_chars++;
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    char longest[64] = "";
    char shortest[64] = "";
    int longest_len = 0;
    int shortest_len = 64;
    
    for (i = 0; i < word_count; i++) {
        int current_len = strlen(words[i]);
        if (current_len > longest_len) {
            longest_len = current_len;
            strncpy(longest, words[i], sizeof(longest) - 1);
            longest[sizeof(longest) - 1] = '\0';
        }
        if (current_len < shortest_len && current_len > 0) {
            shortest_len = current_len;
            strncpy(shortest, words[i], sizeof(shortest) - 1);
            shortest[sizeof(shortest) - 1] = '\0';
        }
    }
    
    if (longest_len > 0) {
        printf("Longest word: \"%s\" (%d chars)\n", longest, longest_len);
    }
    if (shortest_len < 64) {
        printf("Shortest word: \"%s\" (%d chars)\n", shortest, shortest_len);
    }
    
    return 0;
}