//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    int counts[26] = {0};
    
    printf("Enter text to process: ");
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
        return 1;
    }
    
    size_t proc_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            char c = tolower((unsigned char)input[i]);
            if (proc_idx < MAX_INPUT - 1) {
                processed[proc_idx++] = c;
                counts[c - 'a']++;
            }
        }
    }
    processed[proc_idx] = '\0';
    
    printf("Processed text (letters only, lowercase): %s\n", processed);
    
    printf("Letter frequencies:\n");
    int i = 0;
    while (i < 26) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, counts[i]);
        }
        i++;
    }
    
    char reversed[MAX_INPUT];
    size_t rev_idx = 0;
    size_t j = proc_idx;
    do {
        if (j > 0) {
            j--;
            reversed[rev_idx++] = processed[j];
        }
    } while (j > 0);
    reversed[rev_idx] = '\0';
    
    printf("Reversed processed text: %s\n", reversed);
    
    int total_letters = 0;
    for (int k = 0; k < 26; k++) {
        total_letters += counts[k];
    }
    
    if (total_letters > 0) {
        printf("Total letters processed: %d\n", total_letters);
    } else {
        printf("No letters found in input\n");
    }
    
    return 0;
}