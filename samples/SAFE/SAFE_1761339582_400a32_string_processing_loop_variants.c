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
    
    if (len > MAX_INPUT_LEN) {
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
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < (int)strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            }
        } while (m < (int)strlen(words[k]));
        
        printf("\n");
    }
    
    printf("Processing words:\n");
    int n = 0;
    for (n = 0; n < word_count; n++) {
        int char_count = 0;
        int p = 0;
        
        while (words[n][p] != '\0') {
            char_count++;
            p++;
        }
        
        int vowel_count = 0;
        int q = 0;
        do {
            char ch = tolower(words[n][q]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowel_count++;
            }
            q++;
        } while (words[n][q] != '\0');
        
        printf("Word '%s': %d characters, %d vowels\n", words[n], char_count, vowel_count);
    }
    
    printf("Reversed words:\n");
    int r = 0;
    while (r < word_count) {
        int word_len = strlen(words[r]);
        int s, t;
        
        for (s = 0, t = word_len - 1; s < t; s++, t--) {
            char temp = words[r][s];
            words[r][s] = words[r][t];
            words[r][t] = temp;
        }
        
        printf("Reversed: %s\n", words[r]);
        r++;
    }
    
    return 0;
}