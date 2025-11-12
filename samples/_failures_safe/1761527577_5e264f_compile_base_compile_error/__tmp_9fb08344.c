//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int text_len = (int)strlen(text);
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
            strncpy(words[word_count].word, text + start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    char temp[MAX_WORD_LEN];
    strncpy(temp, word, MAX_WORD_LEN - 1);
    temp[MAX_WORD_LEN - 1] = '\0';
    to_lowercase(temp);
    
    for (int i = 0; i < count; i++) {
        char compare[MAX_WORD_LEN];
        strncpy(compare, words[i].word, MAX_WORD_LEN - 1);
        compare[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(compare);
        
        if (strcmp(temp, compare) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    struct WordCount temp_words[MAX_WORDS];
    int unique_count = 0;
    
    for (int i = 0; i < word_count && unique_count < MAX_WORDS; i++) {
        int found_index = find_word_index(temp_words, unique_count, words[i].word);
        if (found_index == -1) {
            strncpy(temp_words[unique_count].word, words[i].word, MAX_WORD_LEN - 1);
            temp_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            temp_words[unique_count].count = 1;
            unique_count++;
        } else {
            temp_words[found_index].count++;
        }
    }
    
    for (int i = 0; i < unique_count; i++) {
        words[i] = temp_words[i];
    }
    
    return unique_count;
}

void sort_words_by_count(struct WordCount words[], int count) {
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

void process_text(const char *text) {
    if (text == NULL) {
        printf("Error: Input text is NULL\n");
        return;
    }
    
    if (strlen(text) == 0) {
        printf("Error: Input text is empty\n");
        return;
    }
    
    if (strlen(text) >= MAX_INPUT_LEN) {
        printf("Error: Input text too long\n");
        return;
    }
    
    struct WordCount words[MAX_WORDS];
    int initial_count = extract_words(text, words, MAX_WORDS);
    
    if (initial_count == 0) {
        printf("No valid words found in the text.\n");
        return;
    }
    
    int unique_count = count_unique_words(words, initial_count);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return;
    }
    
    sort_words_by_count(words, unique_count);
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text