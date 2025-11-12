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
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
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
    
    printf("Processing words:\n");
    int k = 0;
    do {
        if (word_count == 0) break;
        
        int has_upper = 0;
        int has_lower = 0;
        
        char *ch = words[k];
        while (*ch) {
            if (isupper(*ch)) has_upper = 1;
            if (islower(*ch)) has_lower = 1;
            ch++;
        }
        
        printf("Word '%s': ", words[k]);
        if (has_upper && has_lower) {
            printf("Mixed case\n");
        } else if (has_upper) {
            printf("All uppercase\n");
        } else if (has_lower) {
            printf("All lowercase\n");
        } else {
            printf("No letters\n");
        }
        
        k++;
    } while (k < word_count);
    
    printf("Character counts:\n");
    for (int m = 0; m < word_count; m++) {
        int char_count = 0;
        char *p = words[m];
        for (; *p; p++) {
            char_count++;
        }
        printf("'%s' has %d characters\n", words[m], char_count);
    }
    
    return 0;
}