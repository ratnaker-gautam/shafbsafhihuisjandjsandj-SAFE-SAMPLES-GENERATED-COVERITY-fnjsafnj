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
    int vowel_count = 0;
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
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            if (word_count < MAX_WORDS - 1) {
                word_count++;
            } else {
                break;
            }
        }
    }
    
    for (i = 0; i < len; i++) {
        char c = tolower((unsigned char)input[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        }
        if (!isspace((unsigned char)input[i])) {
            char_count++;
        }
    }
    
    printf("Input string: %s\n", input);
    printf("Character count (excluding spaces): %d\n", char_count);
    printf("Vowel count: %d\n", vowel_count);
    printf("Word count: %d\n", word_count);
    
    printf("Words found:\n");
    k = 0;
    do {
        if (k >= word_count) break;
        printf("  %d: %s\n", k + 1, words[k]);
        k++;
    } while (k < word_count);
    
    printf("Words in reverse order:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("  %d: ", word_count - i);
        j = 0;
        while (words[i][j] != '\0') {
            putchar(words[i][j]);
            j++;
        }
        putchar('\n');
    }
    
    return 0;
}