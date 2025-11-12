//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1000
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
        char current = (i < text_len) ? text[i] : ' ';
        
        if (is_valid_char(current)) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < max_words) {
                    strncpy(words[word_count].word, &text[word_start], word_len);
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

int read_input_text(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) {
        return 0;
    }
    
    printf("Enter text (max %d characters): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strlen(buffer) == 0) {
        return 0;
    }
    
    return 1;
}

void display_word_counts(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        printf("No words to display.\n");
        return;
    }
    
    printf("\nWord frequencies (sorted by count):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    if (!read_input_text(input_text, sizeof(input_text))) {
        printf("Error: No input provided or input too long.\n");
        return 1;
    }
    
    int initial_count = extract_words(input_text, words, MAX_WORDS);
    
    if (initial_count == 0) {
        printf("No valid words found in the input.\n");