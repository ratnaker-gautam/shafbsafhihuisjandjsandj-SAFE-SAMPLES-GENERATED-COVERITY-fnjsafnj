//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_chars = 0;
    tp->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

int add_word(struct TextProcessor *tp, const char *word, int len, int capitalized) {
    if (tp->word_count >= MAX_WORDS || len >= MAX_WORD_LEN || len <= 0) {
        return 0;
    }

    for (int i = 0; i < tp->word_count; i++) {
        if (strncmp(tp->words[i].word, word, len) == 0 && tp->words[i].length == len) {
            tp->words[i].count++;
            return 1;
        }
    }

    if (tp->word_count < MAX_WORDS) {
        strncpy(tp->words[tp->word_count].word, word, len);
        tp->words[tp->word_count].word[len] = '\0';
        tp->words[tp->word_count].count = 1;
        tp->words[tp->word_count].length = len;
        tp->words[tp->word_count].is_capitalized = capitalized;
        tp->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL) return;
    
    int input_len = strlen(input);
    if (input_len >= MAX_TEXT_LEN - 1) return;
    
    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, input, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';
    
    char clean_text[MAX_TEXT_LEN];
    int clean_index = 0;
    
    for (int i = 0; buffer[i] != '\0' && clean_index < MAX_TEXT_LEN - 1; i++) {
        if (is_valid_char(buffer[i])) {
            clean_text[clean_index++] = buffer[i];
        }
    }
    clean_text[clean_index] = '\0';
    
    strncpy(tp->processed_text, clean_text, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = clean_index;
    
    char word[MAX_WORD_LEN];
    int word_len = 0;
    int in_word = 0;
    
    for (int i = 0; clean_text[i] != '\0' && tp->total_words < MAX_WORDS; i++) {
        if (isalpha(clean_text[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = clean_text[i];
            }
        } else {
            if (in_word && word_len > 0) {
                word[word_len] = '\0';
                int capitalized = isupper(word[0]) ? 1 : 0;
                if (add_word(tp, word, word_len, capitalized)) {
                    tp->total_words++;
                }
                in_word = 0;
                word_len = 0;
            }
        }
    }
    
    if (in_word && word_len > 0 && tp->total_words < MAX_WORDS) {
        word[word_len] = '\0';
        int capitalized = isupper(word[0]) ? 1 : 0;
        if (add_word(tp, word, word_len, capitalized)) {
            tp->total_words++;
        }
    }
}

void print_statistics(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("'%s': %d occurrences", tp->words[i].word, tp->words[i].count);
        if (tp->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
    
    printf("\nProcessed text: %s\n", tp->processed_text);
}

int main()