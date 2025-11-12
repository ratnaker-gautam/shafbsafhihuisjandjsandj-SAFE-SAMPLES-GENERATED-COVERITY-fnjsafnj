//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int digit_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            if (word_count < MAX_WORDS) {
                words[word_count][j] = input[i];
                j++;
            }
            i++;
        }
        
        if (word_count < MAX_WORDS && j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    for (k = 0; k < len; k++) {
        if (isalpha((unsigned char)input[k])) {
            char_count++;
        } else if (isdigit((unsigned char)input[k])) {
            digit_count++;
        }
    }
    
    printf("Input analysis:\n");
    printf("Total characters: %zu\n", len);
    printf("Alphabetic characters: %d\n", char_count);
    printf("Digit characters: %d\n", digit_count);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Words:\n");
        for (i = 0; i < word_count; i++) {
            printf("  %d: %s\n", i + 1, words[i]);
        }
    }
    
    printf("Character frequency (letters only):\n");
    int freq[26] = {0};
    for (i = 0; i < len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c);
            if (c >= 'a' && c <= 'z') {
                freq[c - 'a']++;
            }
        }
    }
    
    for (i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            printf("  %c: %d\n", 'a' + i, freq[i]);
        }
    }
    
    return 0;
}