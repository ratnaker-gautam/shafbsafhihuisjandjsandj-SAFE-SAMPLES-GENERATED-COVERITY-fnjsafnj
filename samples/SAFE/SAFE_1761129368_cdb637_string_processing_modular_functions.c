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
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int len = strlen(text);
    int i = 0;
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_word_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= sizeof(words[word_count].word)) {
            word_len = sizeof(words[word_count].word) - 1;
        }
        
        strncpy(words[word_count].word, text + start, word_len);
        words[word_count].word[word_len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(struct WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) {
        return;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (words[i].count == 0) {
            continue;
        }
        
        int found_index = find_word_index(words, unique_count, words[i].word);
        if (found_index == -1) {
            if (unique_count != i) {
                strcpy(words[unique_count].word, words[i].word);
                words[unique_count].count = words[i].count;
            }
            unique_count++;
        } else {
            words[found_index].count += words[i].count;
            words[i].count = 0;
        }
    }
    
    *word_count = unique_count;
}

void to_lowercase(char *str) {
    if (str == NULL) {
        return;
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

void normalize_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        to_lowercase(words[i].word);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    
    return strcmp(wa->word, wb->word);
}

void sort_word_counts(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    qsort(words, count, sizeof(struct WordCount), compare_word_counts);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in the input.\n");
        return 1;
    }
    
    normalize_words(words, word_count);
    count_word_frequencies(words, &word_count);
    sort_word_counts(words, word_count);
    
    printf("\nWord frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
    
    return 0;
}