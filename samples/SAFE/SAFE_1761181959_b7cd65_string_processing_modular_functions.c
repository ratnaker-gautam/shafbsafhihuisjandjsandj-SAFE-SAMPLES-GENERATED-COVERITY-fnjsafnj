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

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found_index = find_word_index(words, unique_count, words[i].word);
        if (found_index == -1) {
            if (unique_count < MAX_WORDS) {
                words[unique_count] = words[i];
                unique_count++;
            }
        } else {
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
            if (words[j].count < words[j + 1].count) {
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
    
    return strlen(buffer);
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount all_words[MAX_WORDS];
    struct WordCount unique_words[MAX_WORDS];
    
    int text_length = read_input_text(input_text, sizeof(input_text));
    if (text_length == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int total_words = extract_words(input_text, all_words, MAX_WORDS);
    if (total_words == 0) {
        printf("No words found in the input.\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        unique_words[i] = all_words[i];
    }
    
    int unique_count = count_unique_words(unique_words, total_words);
    
    sort_words_by_count(unique_words, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", total_words);
    printf("Unique words: %d\n\n", unique_count);
    
    printf("Top 10 most frequent words:\n");
    printf("%-15s %s\n", "Word", "Count");
    printf("%-15s %s\n", "----", "-----");
    
    int display_count = (unique_count < 10) ? unique_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s %d\n", unique_words[i].word, unique_words[i].count);
    }
    
    return 0;
}