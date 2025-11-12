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
    
    printf("Enter a string: ");
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
        printf("Empty input string\n");
        return 0;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
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
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            putchar(words[j][k]);
            k++;
        } while (words[j][k] != '\0');
        printf("\n");
    }
    
    printf("Processing words:\n");
    int j = 0;
    while (j < word_count) {
        char processed[MAX_INPUT];
        int k = 0;
        
        for (int pos = 0; words[j][pos] != '\0' && k < MAX_INPUT - 1; pos++) {
            if (isalpha(words[j][pos])) {
                processed[k++] = toupper(words[j][pos]);
            }
        }
        processed[k] = '\0';
        
        printf("Processed word %d: ", j + 1);
        int m = 0;
        while (processed[m] != '\0') {
            putchar(processed[m]);
            m++;
        }
        printf("\n");
        j++;
    }
    
    printf("Word lengths:\n");
    for (int j = 0; j < word_count; j++) {
        int length = 0;
        char *ptr = words[j];
        while (*ptr != '\0') {
            length++;
            ptr++;
        }
        printf("Word %d length: %d\n", j + 1, length);
    }
    
    return 0;
}