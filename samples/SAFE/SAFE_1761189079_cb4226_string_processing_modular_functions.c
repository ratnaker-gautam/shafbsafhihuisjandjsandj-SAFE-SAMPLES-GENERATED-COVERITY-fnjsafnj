//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum(c) || c == '_';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    const char *ptr = text;
    
    while (*ptr != '\0' && word_count < max_words) {
        while (*ptr != '\0' && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        const char *start = ptr;
        while (*ptr != '\0' && is_word_char(*ptr)) {
            ptr++;
        }
        
        size_t len = ptr - start;
        if (len >= sizeof(words[word_count].word)) {
            len = sizeof(words[word_count].word) - 1;
        }
        
        strncpy(words[word_count].word, start, len);
        words[word_count].word[len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }
    
    return word_count;
}

void to_lowercase_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (char *p = words[i].word; *p != '\0'; p++) {
            *p = tolower(*p);
        }
    }
}

void sort_words_by_count(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int initial_word_count = extract_words(input, words, MAX_WORDS);
    if (initial_word_count == 0) {
        printf("No words found in the input.\n");
        return 1;
    }
    
    to_lowercase_words(words, initial_word_count);
    int unique_word_count = count_unique_words(words, initial_word_count);
    sort_words_by_count(words, unique_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < unique_word_count && i < 20; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    if (unique_word_count > 20) {
        printf("... and %d more words\n", unique_word_count - 20);
    }
    
    return 0;
}