//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 50
#define MAX_LINE_LEN 200
#define MAX_WORDS 100

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordStats words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int word_len = 0;
    
    while (text[pos] != '\0' && word_count < max_words) {
        if (is_valid_char(text[pos])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
                memset(current_word, 0, sizeof(current_word));
            }
            
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len] = tolower((unsigned char)text[pos]);
                word_len++;
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i].word, current_word) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    words[word_count].length = word_len;
                    word_count++;
                }
                
                in_word = 0;
            }
        }
        pos++;
    }
    
    if (in_word && word_len > 0 && word_count < max_words) {
        current_word[word_len] = '\0';
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[word_count].count = 1;
            words[word_count].length = word_len;
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words_by_frequency(struct WordStats words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordStats temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_stats(const struct WordStats words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("%-20s %-10s %-10s\n", "Word", "Count", "Length");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-20s %-10d %-10d\n", words[i].word, words[i].count, words[i].length);
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN] = {0};
    struct WordStats words[MAX_WORDS] = {0};
    
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
    
    sort_words_by_frequency(words, word_count);
    print_word_stats(words, word_count);
    
    return 0;
}