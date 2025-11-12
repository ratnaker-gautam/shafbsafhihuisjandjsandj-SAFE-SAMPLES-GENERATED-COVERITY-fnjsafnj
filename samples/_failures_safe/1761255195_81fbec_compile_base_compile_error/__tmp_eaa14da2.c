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
        char current = text[i];
        
        if (i < text_len && is_valid_char(current)) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < max_words) {
                    strncpy(words[word_count].word, text + word_start, word_len);
                    words[word_count].word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        words[word_count].word[j] = tolower((unsigned char)words[word_count].word[j]);
                    }
                    
                    words[word_count].count = 1;
                    word_count++;
                }
                word_start = -1;
            }
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
            int existing_index = find_word_index(words, unique_count, words[i].word);
            
            if (existing_index == -1) {
                if (unique_count != i) {
                    strcpy(words[unique_count].word, words[i].word);
                    words[unique_count].count = words[i].count;
                }
                unique_count++;
            } else {
                words[existing_index].count += words[i].count;
            }
        }
    }
    
    *word_count = unique_count;
}

void sort_words_by_frequency(struct WordCount words[], int count) {
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

void process_text_analysis(const char *input_text) {
    if (input_text == NULL || strlen(input_text) == 0) {
        printf("No text to analyze.\n");
        return;
    }
    
    struct WordCount words[MAX_WORDS];
    int word_count = extract_words(input_text, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the text.\n");
        return;
    }
    
    count_word_frequencies(words, &word_count);
    sort_words_by_frequency(words, word_count);
    
    printf("Word frequency analysis:\n");
    printf("=======================\n");
    
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count > 10) {
        printf("... and %d more words\n", word_count - 10);
    }
    
    printf("Total unique words: %d\n", word_count);
}

int main() {
    char input_text[MAX_LINE_LEN * 5];
    char buffer[MAX_LINE_LEN];
    
    printf("Enter text for word frequency analysis (empty line to finish):\n");
    
    input_text[0] = '\0';
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') {
            break;
        }
        
        size_t buffer_len = strlen(buffer);
        if (buffer_len > 0 && buffer[buffer_len - 1] == '\n') {
            buffer[buffer_len - 1] = '\