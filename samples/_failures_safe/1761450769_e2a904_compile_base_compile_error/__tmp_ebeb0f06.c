//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_chars = 0;
    tp->total_words = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, input, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    tp->total_chars = strlen(buffer);
    char *ptr = buffer;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr && tp->total_words < MAX_WORDS) {
        if (is_word_char(*ptr)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *ptr;
            }
        } else {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int idx = find_word_index(tp, current_word);
                if (idx == -1 && tp->word_count < MAX_WORDS) {
                    idx = tp->word_count;
                    strncpy(tp->words[idx].word, current_word, MAX_WORD_LEN - 1);
                    tp->words[idx].word[MAX_WORD_LEN - 1] = '\0';
                    tp->words[idx].length = word_len;
                    tp->words[idx].is_capitalized = isupper((unsigned char)current_word[0]);
                    tp->words[idx].count = 1;
                    tp->word_count++;
                } else if (idx != -1) {
                    tp->words[idx].count++;
                }
                tp->total_words++;
                in_word = 0;
            }
        }
        ptr++;
    }
    
    if (in_word && word_len > 0 && tp->total_words < MAX_WORDS) {
        current_word[word_len] = '\0';
        int idx = find_word_index(tp, current_word);
        if (idx == -1 && tp->word_count < MAX_WORDS) {
            idx = tp->word_count;
            strncpy(tp->words[idx].word, current_word, MAX_WORD_LEN - 1);
            tp->words[idx].word[MAX_WORD_LEN - 1] = '\0';
            tp->words[idx].length = word_len;
            tp->words[idx].is_capitalized = isupper((unsigned char)current_word[0]);
            tp->words[idx].count = 1;
            tp->word_count++;
        } else if (idx != -1) {
            tp->words[idx].count++;
        }
        tp->total_words++;
    }
    
    strncpy(tp->processed_text, buffer, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    
    return 1;
}

void print_word_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s: %d occurrences", tp->words[i].word, tp->words[i].count);
        if (tp->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input