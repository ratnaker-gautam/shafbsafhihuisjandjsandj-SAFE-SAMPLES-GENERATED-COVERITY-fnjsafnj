//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) {
        return 0;
    }
    strcpy(buffer, input);
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && *word_count < MAX_WORDS) {
        if (strlen(token) >= MAX_WORD_LEN) {
            token = strtok(NULL, " \t\n\r.,;:!?");
            continue;
        }
        
        char clean_word[MAX_WORD_LEN];
        int clean_len = 0;
        
        for (int i = 0; token[i] && clean_len < MAX_WORD_LEN - 1; i++) {
            if (is_valid_char(token[i])) {
                clean_word[clean_len++] = token[i];
            }
        }
        clean_word[clean_len] = '\0';
        
        if (clean_len > 0) {
            to_lower_case(clean_word);
            
            int index = find_word_index(words, *word_count, clean_word);
            if (index >= 0) {
                words[index].count++;
            } else {
                strcpy(words[*word_count].word, clean_word);
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
        
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    return 1;
}

void sort_words(struct WordCount words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        printf("Input too long or processing error.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%2d. %-20s (%d occurrence%s)\n", 
               i + 1, words[i].word, words[i].count,
               words[i].count == 1 ? "" : "s");
    }
    
    return 0;
}