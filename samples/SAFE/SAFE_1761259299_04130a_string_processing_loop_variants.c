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
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        char *ptr = words[j];
        while (*ptr) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int k = 0;
    do {
        int word_len = strlen(words[k]);
        int vowel_count = 0;
        
        for (int m = 0; m < word_len; m++) {
            char c = tolower(words[k][m]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word '%s' has %d vowels\n", words[k], vowel_count);
        k++;
    } while (k < word_count);
    
    printf("Reversed words:\n");
    
    for (int n = 0; n < word_count; n++) {
        int word_len = strlen(words[n]);
        char reversed[MAX_INPUT];
        
        int idx = 0;
        for (int p = word_len - 1; p >= 0; p--) {
            if (idx < MAX_INPUT - 1) {
                reversed[idx++] = words[n][p];
            }
        }
        reversed[idx] = '\0';
        
        printf("%s -> %s\n", words[n], reversed);
    }
    
    return 0;
}