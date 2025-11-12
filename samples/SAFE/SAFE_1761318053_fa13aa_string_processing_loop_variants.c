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
    char temp[MAX_WORD_LEN + 1];
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Original words (%d found):\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("%s ", words[idx]);
    }
    printf("\n");
    
    printf("Words in reverse order:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        printf("%s ", words[idx]);
    }
    printf("\n");
    
    printf("Words with vowels capitalized:\n");
    for (int idx = 0; idx < word_count; idx++) {
        char *word = words[idx];
        for (int pos = 0; word[pos] != '\0'; pos++) {
            char c = word[pos];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                putchar(toupper((unsigned char)c));
            } else {
                putchar(c);
            }
        }
        putchar(' ');
    }
    printf("\n");
    
    printf("Word lengths:\n");
    int total_length = 0;
    int idx = 0;
    do {
        int word_len = strlen(words[idx]);
        printf("%s: %d ", words[idx], word_len);
        total_length += word_len;
        idx++;
    } while (idx < word_count);
    printf("\nTotal characters: %d\n", total_length);
    
    printf("Words sorted alphabetically:\n");
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                strncpy(temp, words[j], MAX_WORD_LEN);
                temp[MAX_WORD_LEN] = '\0';
                strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                words[j][MAX_WORD_LEN] = '\0';
                strncpy(words[j + 1], temp, MAX_WORD_LEN);
                words[j + 1][MAX_WORD_LEN] = '\0';
            }
        }
    }
    
    k = 0;
    while (k < word_count) {
        printf("%s ", words[k]);
        k++;
    }
    printf("\n");
    
    return 0;
}