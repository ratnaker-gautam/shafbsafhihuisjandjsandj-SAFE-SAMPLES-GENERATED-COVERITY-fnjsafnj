//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT + 1];
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
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
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
        words[word_count][j] = '\0';
        
        if (j > 0) {
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
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        int has_alpha = 0;
        int has_digit = 0;
        
        char *ptr = words[k];
        while (*ptr != '\0') {
            if (isalpha((unsigned char)*ptr)) {
                has_alpha = 1;
            } else if (isdigit((unsigned char)*ptr)) {
                has_digit = 1;
            }
            ptr++;
        }
        
        if (has_alpha && has_digit) {
            int n = 0;
            char temp[MAX_WORD_LEN + 1];
            
            for (int p = 0; words[k][p] != '\0'; p++) {
                if (isalpha((unsigned char)words[k][p])) {
                    temp[n] = toupper((unsigned char)words[k][p]);
                    n++;
                }
            }
            temp[n] = '\0';
            
            if (n > 0) {
                printf("Mixed word '%s' -> '%s'\n", words[k], temp);
                processed++;
            }
        }
    }
    
    printf("Processed %d mixed words\n", processed);
    
    return 0;
}