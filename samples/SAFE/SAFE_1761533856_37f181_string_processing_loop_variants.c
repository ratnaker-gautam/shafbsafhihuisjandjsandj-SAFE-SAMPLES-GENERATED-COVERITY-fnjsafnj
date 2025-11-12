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
    int i, j, k;
    
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
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        char *ptr = words[k];
        while (*ptr) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        if (word_len == 0) continue;
        
        int has_alpha = 0;
        char *c = words[k];
        do {
            if (isalpha(*c)) {
                has_alpha = 1;
                break;
            }
            c++;
        } while (*c);
        
        if (has_alpha) {
            char temp[64];
            int idx = 0;
            
            for (int pos = 0; pos < word_len && idx < 63; pos++) {
                if (isalpha(words[k][pos])) {
                    temp[idx] = toupper(words[k][pos]);
                    idx++;
                }
            }
            temp[idx] = '\0';
            
            if (idx > 0) {
                strncpy(words[k], temp, 63);
                words[k][63] = '\0';
                processed++;
            }
        }
    }
    
    printf("Modified %d words\n", processed);
    
    printf("Final word list:\n");
    k = 0;
    while (k < word_count) {
        printf("%s\n", words[k]);
        k++;
    }
    
    return 0;
}