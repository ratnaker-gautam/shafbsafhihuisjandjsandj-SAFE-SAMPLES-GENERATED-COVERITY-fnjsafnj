//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
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
        printf("Empty input\n");
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
        if (word_len > 0 && word_len <= MAX_INPUT_LEN) {
            strncpy(words[word_count], input + i, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: ", i + 1);
        
        int word_len = strlen(words[i]);
        for (j = 0; j < word_len; j++) {
            putchar(words[i][j]);
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        int has_alpha = 0;
        
        for (j = 0; j < word_len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                has_alpha = 1;
                break;
            }
        }
        
        if (has_alpha) {
            int vowel_count = 0;
            int consonant_count = 0;
            
            for (j = 0; j < word_len; j++) {
                char c = tolower((unsigned char)words[i][j]);
                if (isalpha((unsigned char)c)) {
                    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                        vowel_count++;
                    } else {
                        consonant_count++;
                    }
                }
            }
            
            printf("Word '%s': vowels=%d, consonants=%d\n", words[i], vowel_count, consonant_count);
            processed++;
        }
    }
    
    printf("Processed %d words with alphabetic characters\n", processed);
    
    return 0;
}