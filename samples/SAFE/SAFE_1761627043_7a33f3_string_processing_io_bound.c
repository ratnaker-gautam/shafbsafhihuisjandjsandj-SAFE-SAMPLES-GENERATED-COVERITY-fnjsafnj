//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text to process: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < 64) {
            strncpy(words[word_count], token, 63);
            words[word_count][63] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Word lengths: ");
    for (int i = 0; i < word_count; i++) {
        printf("%zu", strlen(words[i]));
        if (i < word_count - 1) printf(", ");
    }
    printf("\n");
    
    printf("Uppercase words: ");
    int uppercase_count = 0;
    for (int i = 0; i < word_count; i++) {
        int all_upper = 1;
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (!isupper((unsigned char)words[i][j])) {
                all_upper = 0;
                break;
            }
        }
        if (all_upper && strlen(words[i]) > 0) {
            if (uppercase_count > 0) printf(", ");
            printf("%s", words[i]);
            uppercase_count++;
        }
    }
    if (uppercase_count == 0) printf("none");
    printf("\n");
    
    printf("Longest word(s): ");
    size_t max_len = 0;
    for (int i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
        }
    }
    
    int first_longest = 1;
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == max_len) {
            if (!first_longest) printf(", ");
            printf("%s", words[i]);
            first_longest = 0;
        }
    }
    printf(" (%zu characters)\n", max_len);
    
    char search_term[64];
    printf("\nEnter a word to search for: ");
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        fprintf(stderr, "Error reading search term\n");
        return 1;
    }
    
    len = strlen(search_term);
    if (len > 0 && search_term[len - 1] == '\n') {
        search_term[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No search term provided\n");
        return 1;
    }
    
    int found_count = 0;
    printf("Occurrences of '%s': ", search_term);
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], search_term) == 0) {
            if (found_count > 0) printf(", ");
            printf("position %d", i + 1);
            found_count++;
        }
    }
    if (found_count == 0) printf("not found");
    printf("\n");
    
    return 0;
}