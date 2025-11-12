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
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void process_word(struct TextAnalyzer *analyzer, char *word) {
    if (word[0] == '\0') return;
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        word[MAX_WORD_LEN - 1] = '\0';
        len = MAX_WORD_LEN - 1;
    }
    
    int found = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            found = 1;
            break;
        }
    }
    
    if (!found && analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
    
    analyzer->total_words++;
    
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
    if (len < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = len;
    }
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    if (strlen(text) >= sizeof(buffer)) {
        return;
    }
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (is_word_character(buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = buffer[i];
            }
        } else {
            if (in_word) {
                word[word_pos] = '\0';
                process_word(analyzer, word);
                in_word = 0;
                word_pos = 0;
            }
        }
    }
    
    if (in_word && word_pos > 0) {
        word[word_pos] = '\0';
        process_word(analyzer, word);
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency (top 10):\n");
    
    struct WordStats sorted[MAX_WORDS];
    int count = analyzer->word_count;
    if (count > MAX_WORDS) count = MAX_WORDS;
    
    for (int i = 0; i < count; i++) {
        sorted[i] = analyzer->words[i];
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (sorted[j].count > sorted[i].count) {
                struct WordStats temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    int limit = count < 10 ? count : 10;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d occurrences\n", sorted[i].word, sorted[i].count);
    }
}

int main() {
    struct Text