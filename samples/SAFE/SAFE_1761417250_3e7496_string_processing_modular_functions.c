//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 50
#define MAX_LINE_LEN 200
#define MAX_WORDS 1000

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int text_len = strlen(text);
    int i = 0;
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && !is_valid_char(text[i])) {
            i++;
        }
        
        if (i >= text_len) {
            break;
        }
        
        int start = i;
        while (i < text_len && is_valid_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count].word, &text[start], word_len);
            words[word_count].word[word_len] = '\0';
            
            for (int j = 0; j < word_len; j++) {
                words[word_count].word[j] = tolower((unsigned char)words[word_count].word[j]);
            }
            
            words[word_count].count = 1;
            word_count++;
        }
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
        if (words[i].count > 0) {
            int found_idx = find_word_index(words, unique_count, words[i].word);
            
            if (found_idx >= 0) {
                words[found_idx].count++;
                words[i].count = 0;
            } else {
                if (unique_count != i) {
                    words[unique_count] = words[i];
                    words[i].count = 0;
                }
                unique_count++;
            }
        }
    }
    
    *word_count = unique_count;
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_word_frequencies(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input_text, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    count_word_frequencies(words, &word_count);
    
    if (word_count > 1) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
    }
    
    print_word_frequencies(words, word_count);
    
    return 0;
}