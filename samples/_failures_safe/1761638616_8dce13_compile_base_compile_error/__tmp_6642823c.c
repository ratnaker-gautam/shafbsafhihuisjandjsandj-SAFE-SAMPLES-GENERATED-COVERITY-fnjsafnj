//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

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

int find_word_index(const struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL || count <= 0) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(const char *text, struct WordCount unique_words[], int max_unique) {
    if (text == NULL || unique_words == NULL || max_unique <= 0) {
        return 0;
    }
    
    struct WordCount all_words[MAX_WORDS];
    int total_words = extract_words(text, all_words, MAX_WORDS);
    
    if (total_words <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < total_words && unique_count < max_unique; i++) {
        int existing_index = find_word_index(unique_words, unique_count, all_words[i].word);
        
        if (existing_index >= 0) {
            unique_words[existing_index].count++;
        } else {
            strncpy(unique_words[unique_count].word, all_words[i].word, MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_words_by_frequency(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
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

int get_user_input(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) {
        return 0;
    }
    
    printf("Enter text to analyze (max %d characters): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input_text[MAX_INPUT_LEN];
    
    if (!get_user_input(input_text, MAX_INPUT_LEN)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    struct WordCount unique_words[MAX_WORDS];
    int unique_count = count_unique_words(input_text, unique_words, MAX_WORDS);
    
    if (unique_count == 0) {
        printf("No valid words found in the input\n");
        return 0;
    }
    
    sort_words_by_frequency(unique_words, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < unique_count; i++) {
        printf("%-20s %d\n", unique_words[i].word