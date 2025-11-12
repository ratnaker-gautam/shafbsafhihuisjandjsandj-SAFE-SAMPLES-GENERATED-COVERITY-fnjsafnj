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

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found_index = find_word_index(words, i, words[i].word);
        if (found_index == i) {
            unique_count++;
        } else if (found_index >= 0) {
            words[found_index].count++;
        }
    }
    
    return unique_count;
}

void sort_words_by_count(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
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

void print_word_stats(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    int display_count = count > 10 ? 10 : count;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    
    if (count > 10) {
        printf("... and %d more words\n", count - 10);
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
    
    int total_words = extract_words(input_text, words, MAX_WORDS);
    if (total_words == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    int unique_words = count_unique_words(words, total_words);
    
    struct WordCount unique_list[MAX_WORDS];
    int unique_index = 0;
    
    for (int i = 0; i < total_words && unique_index < MAX_WORDS; i++) {
        if (find_word_index(unique_list, unique_index, words[i].word) == -1) {
            strcpy(unique_list[unique_index].word, words[i].word);
            unique_list[unique_index].count = words[i].count;
            unique_index++;
        }
    }
    
    sort_words_by_count(unique