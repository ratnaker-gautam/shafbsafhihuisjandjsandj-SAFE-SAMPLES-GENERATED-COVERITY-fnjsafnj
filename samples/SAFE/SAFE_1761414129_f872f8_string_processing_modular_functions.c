//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 50
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN + 1];
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
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN + 1];
            strncpy(temp_word, &text[start], word_len);
            temp_word[word_len] = '\0';
            
            for (int j = 0; j < word_len; j++) {
                temp_word[j] = tolower((unsigned char)temp_word[j]);
            }
            
            int found = 0;
            for (int j = 0; j < word_count; j++) {
                if (strcmp(words[j].word, temp_word) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN);
                words[word_count].word[MAX_WORD_LEN] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
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

void print_word_counts(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input_text[1024];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
    
    sort_words(words, word_count);
    print_word_counts(words, word_count);
    
    return 0;
}