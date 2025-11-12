//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 1024

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

struct TextProcessor {
    char input_buffer[MAX_LINE_LEN];
    char output_buffer[MAX_LINE_LEN];
    int processed_lines;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
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

void initialize_processor(struct TextProcessor *processor) {
    processor->input_buffer[0] = '\0';
    processor->output_buffer[0] = '\0';
    processor->processed_lines = 0;
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.';
}

int safe_strlen(const char *str, int max_len) {
    int len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (analyzer->word_count >= MAX_WORDS || length <= 0 || length >= MAX_WORD_LEN) {
        return;
    }

    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }

    safe_strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN);
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = length;
    analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    
    analyzer->word_count++;
    analyzer->unique_words++;
}

void process_line(struct TextProcessor *processor, struct TextAnalyzer *analyzer) {
    char temp_word[MAX_WORD_LEN];
    int word_pos = 0;
    int line_len = safe_strlen(processor->input_buffer, MAX_LINE_LEN);

    for (int i = 0; i <= line_len; i++) {
        char c = processor->input_buffer[i];
        
        if (is_valid_char(c) && !isspace((unsigned char)c) && c != '.' && word_pos < MAX_WORD_LEN - 1) {
            temp_word[word_pos++] = tolower((unsigned char)c);
        } else if (word_pos > 0) {
            temp_word[word_pos] = '\0';
            add_word(analyzer, temp_word, word_pos);
            analyzer->total_words++;
            word_pos = 0;
        }
        
        if (c == '\0') break;
    }
    
    processor->processed_lines++;
}

void generate_report(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Report:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    for (int i = 0; i < analyzer->word_count && i < 10; i++) {
        int max_index = i;
        for (int j = i + 1; j < analyzer->word_count; j++) {
            if (analyzer->words[j].count > analyzer->words[max_index].count) {
                max_index = j;
            }
        }
        
        if (max_index != i) {
            struct WordStats temp = analyzer->words[i];
            analyzer->words[i] = analyzer->words[max_index];
            analyzer->words[max_index] = temp;
        }
        
        printf("%s: %d\n", analyzer