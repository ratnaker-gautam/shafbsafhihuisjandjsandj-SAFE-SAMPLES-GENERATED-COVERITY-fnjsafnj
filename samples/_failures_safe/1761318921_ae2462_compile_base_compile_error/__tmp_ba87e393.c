//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

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
    int total_chars;
    int longest_word_len;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->total_chars = 0;
    analyzer->longest_word_len = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len, int capitalized) {
    if (len <= 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return 1;
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = capitalized;
        analyzer->word_count++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    int word_start_cap = 0;
    
    for (int i = 0; text[i] != '\0' && i < MAX_LINE_LEN * 10; i++) {
        char c = text[i];
        
        if (is_word_char(c)) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
                word_start_cap = isupper((unsigned char)c) ? 1 : 0;
            }
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (add_word(analyzer, buffer, buf_pos, word_start_cap)) {
                    analyzer->total_words++;
                    analyzer->total_chars += buf_pos;
                    if (buf_pos > analyzer->longest_word_len) {
                        analyzer->longest_word_len = buf_pos;
                    }
                }
                in_word = 0;
                buf_pos = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        if (add_word(analyzer, buffer, buf_pos, word_start_cap)) {
            analyzer->total_words++;
            analyzer->total_chars += buf_pos;
            if (buf_pos > analyzer->longest_word_len) {
                analyzer->longest_word_len = buf_pos;
            }
        }
    }
}

int compare_word_freq(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_analysis(struct TextAnalyzer *analyzer) {
    if (analyzer->total_words == 0) {
        printf("No words found in text.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Longest word length: %d\n", analyzer->longest_word_len);
    printf("Average word length: %.2f\n", 
           (float)analyzer->total_chars / analyzer->total_words);
    
    printf