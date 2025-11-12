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
    int word_start = -1;
    
    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (is_valid_char(c) && word_start == -1) {
            word_start = i;
        } else if ((!is_valid_char(c) || c == '\0') && word_start != -1) {
            int word_len = i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                if (word_count < max_words) {
                    strncpy(words[word_count].word, &text[word_start], word_len);
                    words[word_count].word[word_len] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
            word_start = -1;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_word_frequencies(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
        
        int existing_index = find_word_index(words, unique_count, words[i].word);
        
        if (existing_index != -1) {
            words[existing_index].count++;
        } else {
            if (unique_count < i) {
                strcpy(words[unique_count].word, words[i].word);
                words[unique_count].count = words[i].count;
            }
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_words_by_frequency(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
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
    
    int raw_word_count = extract_words(input_text, words, MAX_WORDS);
    
    if (raw_word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    int unique_count = process_word_frequencies(words, raw_word_count);
    
    sort_words_by_frequency(words, unique_count);
    
    printf("\nWord frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    
    for (int i = 0; i < unique_count; i++) {
        if (i < 10) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
    }
    
    if (unique_count > 10) {
        printf("... and %d more words\n", unique_count - 10);
    }
    
    return 0;
}