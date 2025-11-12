//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int unique_words;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    char normalized[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        return;
    }
    strcpy(normalized, word);
    to_lower_case(normalized);
    
    int index = find_word_index(analyzer, normalized);
    
    if (index == -1) {
        if (analyzer->word_count < MAX_WORDS) {
            strcpy(analyzer->words[analyzer->word_count].word, normalized);
            analyzer->words[analyzer->word_count].count = 1;
            analyzer->words[analyzer->word_count].length = strlen(normalized);
            analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
            
            if (analyzer->words[analyzer->word_count].length > analyzer->longest_word_length) {
                analyzer->longest_word_length = analyzer->words[analyzer->word_count].length;
            }
            
            analyzer->word_count++;
            analyzer->unique_words++;
        }
    } else {
        analyzer->words[index].count++;
        if (is_capitalized) {
            analyzer->words[index].is_capitalized = 1;
        }
    }
    
    analyzer->total_words++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    int word_start_capitalized = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (is_word_character(text[i])) {
            if (!in_word) {
                in_word = 1;
                word_start_capitalized = isupper((unsigned char)text[i]);
                buf_index = 0;
            }
            
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (in_word) {
                buffer[buf_index] = '\0';
                if (buf_index > 0) {
                    add_word(analyzer, buffer, word_start_capitalized);
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, word_start_capitalized);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\n");
    
    qsort(analyzer->words, analyzer->word_count, sizeof(struct WordStats), compare_word_count);
    
    printf("Top 10 most frequent words:\n");
    printf("%-20s %-8s %-8s %s\n", "Word", "Count", "Length", "Capitalized");
    printf("------------------------------------------------------------\n");
    
    int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
    for (int i = 0; i <