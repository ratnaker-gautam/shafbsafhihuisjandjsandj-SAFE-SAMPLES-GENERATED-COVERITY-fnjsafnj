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
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    const char *p = text;
    
    while (*p != '\0' && word_count < max_words) {
        while (*p != '\0' && !is_word_char(*p)) {
            p++;
        }
        
        if (*p == '\0') {
            break;
        }
        
        const char *start = p;
        while (*p != '\0' && is_word_char(*p)) {
            p++;
        }
        
        size_t len = p - start;
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

void count_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) {
            continue;
        }
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > 0 && strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
}

void to_lowercase(char *str) {
    if (str == NULL) {
        return;
    }
    
    for (char *p = str; *p != '\0'; p++) {
        *p = tolower((unsigned char)*p);
    }
}

void normalize_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    
    return strcmp(wa->word, wb->word);
}

void sort_words_by_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
}

void print_word_frequencies(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    printf("Word frequencies:\n");
    printf("-----------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in the input.\n");
        return 1;
    }
    
    normalize_words(words, word_count);
    count_word_frequency(words, word_count);
    
    int valid_word_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            valid_word_count++;
        }
    }
    
    if (valid_word_count == 0) {
        printf("No valid words found.\n");
        return 1;
    }
    
    sort_words_by_frequency(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}