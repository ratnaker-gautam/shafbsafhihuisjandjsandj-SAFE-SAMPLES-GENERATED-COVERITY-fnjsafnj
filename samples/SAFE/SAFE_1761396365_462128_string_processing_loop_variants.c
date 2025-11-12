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
    
    printf("Enter a string: ");
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
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < strlen(words[k]));
        
        printf("\n");
    }
    
    printf("Processing words with different loop styles:\n");
    
    int n = 0;
    while (n < word_count) {
        int vowel_count = 0;
        int p = 0;
        
        for (p = 0; p < strlen(words[n]); p++) {
            char ch = tolower(words[n][p]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word '%s' has %d vowels\n", words[n], vowel_count);
        n++;
    }
    
    printf("Reversed words:\n");
    
    int q = 0;
    while (q < word_count) {
        char reversed[MAX_WORD_LEN + 1];
        int r = strlen(words[q]) - 1;
        int s = 0;
        
        while (r >= 0 && s < MAX_WORD_LEN) {
            reversed[s] = words[q][r];
            s++;
            r--;
        }
        reversed[s] = '\0';
        
        printf("Reversed: %s\n", reversed);
        q++;
    }
    
    return 0;
}