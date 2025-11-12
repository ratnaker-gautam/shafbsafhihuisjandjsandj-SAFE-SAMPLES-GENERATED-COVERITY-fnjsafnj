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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
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

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    int len = strlen(text);
    int i = 0;
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        if (is_word_char(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                to_lower_case(buffer);
                
                int existing_index = find_word_index(words, *word_count, buffer);
                if (existing_index >= 0) {
                    words[existing_index].count++;
                } else {
                    if (strlen(buffer) > 0) {
                        strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                buf_index = 0;
            }
        }
        i++;
    }
    
    if (buf_index > 0 && *word_count < MAX_WORDS) {
        buffer[buf_index] = '\0';
        to_lower_case(buffer);
        int existing_index = find_word_index(words, *word_count, buffer);
        if (existing_index >= 0) {
            words[existing_index].count++;
        } else {
            if (strlen(buffer) > 0) {
                strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    
    return 0;
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
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (extract_words(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in the input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}