//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
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
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (k = 0; k < word_count; k++) {
        printf("%s ", words[k]);
    }
    printf("\n");
    
    printf("Words in reverse order:\n");
    k = word_count - 1;
    do {
        printf("%s ", words[k]);
        k--;
    } while (k >= 0);
    printf("\n");
    
    printf("Words with vowels capitalized:\n");
    for (k = 0; k < word_count; k++) {
        char *word = words[k];
        int pos = 0;
        while (word[pos] != '\0') {
            char c = word[pos];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                putchar(toupper((unsigned char)c));
            } else {
                putchar(c);
            }
            pos++;
        }
        putchar(' ');
    }
    printf("\n");
    
    printf("Word lengths:\n");
    k = 0;
    while (k < word_count) {
        size_t word_len = strlen(words[k]);
        printf("%s: %zu\n", words[k], word_len);
        k++;
    }
    
    return 0;
}