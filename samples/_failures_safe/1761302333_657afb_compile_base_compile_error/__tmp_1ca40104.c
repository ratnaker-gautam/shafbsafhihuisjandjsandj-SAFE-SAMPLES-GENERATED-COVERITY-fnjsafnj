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
    double frequency;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char text[MAX_TEXT_LEN];
    int text_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->text_length = 0;
    analyzer->text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (analyzer == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_TEXT_LEN) return 0;
    
    if (!safe_strncpy(analyzer->text, input, MAX_TEXT_LEN)) return 0;
    analyzer->text_length = (int)input_len;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= analyzer->text_length; i++) {
        char c = analyzer->text[i];
        
        if (!is_valid_char(c)) return 0;
        
        if (isalnum((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = (char)tolower((unsigned char)c);
                in_word = 1;
            } else {
                return 0;
            }
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            analyzer->total_words++;
            
            int found = 0;
            for (int j = 0; j < analyzer->word_count; j++) {
                if (strcmp(analyzer->words[j].word, buffer) == 0) {
                    analyzer->words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && analyzer->word_count < MAX_WORDS) {
                if (safe_strncpy(analyzer->words[analyzer->word_count].word, buffer, MAX_WORD_LEN)) {
                    analyzer->words[analyzer->word_count].count = 1;
                    analyzer->words[analyzer->word_count].length = buf_pos;
                    analyzer->word_count++;
                }
            }
            
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    if (analyzer->total_words > 0) {
        for (int i = 0; i < analyzer->word_count; i++) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / (double)analyzer->total_words;
        }
    }
    
    return 1;
}

int compare_word_count(const void *a, const void *b) {
    const struct WordStats *wa = (const struct WordStats *)a;
    const struct WordStats *wb = (const struct WordStats *)b;
    return wb->count - wa->count;
}

void display_results(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->word_count == 0) return;
    
    struct WordStats sorted[MAX_WORDS];
    for (int i = 0; i < analyzer->word_count; i++) {
        sorted[i] = analyzer->words[i];
    }
    
    qsort(sorted, (size_t)analyzer->word_count, sizeof(struct WordStats), compare_word_count);
    
    printf("Text analysis results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nTop 10 most frequent words:\n");
    printf("%-15s %-8s %-12s %s\n", "Word", "Count", "Length", "Frequency");
    printf("------------------------------------------------\n");
    
    int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
    for (int i = 0; i < limit; i++) {
        printf("%-15s %-8d %-12d %.4f\n", 
               sorted[i].word,