//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
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
        if (word_len >= 64) {
            word_len = 63;
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
        int k = 0;
        while (words[j][k] != '\0') {
            words[j][k] = toupper(words[j][k]);
            k++;
        }
    }
    
    printf("Uppercase words:\n");
    int j = 0;
    do {
        printf("%s\n", words[j]);
        j++;
    } while (j < word_count);
    
    printf("Reversed order:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        char reversed[64];
        int pos = 0;
        int word_len = strlen(words[idx]);
        
        for (int k = word_len - 1; k >= 0; k--) {
            if (pos < 63) {
                reversed[pos++] = words[idx][k];
            }
        }
        reversed[pos] = '\0';
        printf("%s\n", reversed);
    }
    
    int total_chars = 0;
    int idx = 0;
    while (idx < word_count) {
        total_chars += strlen(words[idx]);
        idx++;
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}