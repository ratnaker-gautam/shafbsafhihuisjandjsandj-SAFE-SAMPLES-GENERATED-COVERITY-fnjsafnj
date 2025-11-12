//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextProcessor {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    int longest_word_length;
    int shortest_word_length;
    char most_vowels_word[MAX_WORD_LEN];
    int max_vowels;
};

void initialize_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_vowels = 0;
    proc->total_consonants = 0;
}

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    initialize_processor(&analyzer->processor);
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    analyzer->max_vowels = 0;
    memset(analyzer->most_vowels_word, 0, MAX_WORD_LEN);
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int count_vowels(const char *str) {
    int count = 0;
    if (str == NULL) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (is_vowel(str[i])) {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str) {
    int count = 0;
    if (str == NULL) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i]) && !is_vowel(str[i])) {
            count++;
        }
    }
    return count;
}

int process_word(struct TextProcessor *proc, const char *word) {
    if (proc == NULL || word == NULL) {
        return 0;
    }
    if (proc->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN || len == 0) {
        return 0;
    }
    
    struct WordInfo *info = &proc->words[proc->word_count];
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    info->length = (int)len;
    info->vowel_count = count_vowels(word);
    info->consonant_count = count_consonants(word);
    
    proc->total_vowels += info->vowel_count;
    proc->total_consonants += info->consonant_count;
    proc->word_count++;
    
    return 1;
}

void analyze_text(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    analyzer->max_vowels = 0;
    
    for (int i = 0; i < analyzer->processor.word_count; i++) {
        struct WordInfo *info = &analyzer->processor.words[i];
        
        if (info->length > analyzer->longest_word_length) {
            analyzer->longest_word_length = info->length;
        }
        
        if (info->length < analyzer->shortest_word_length) {
            analyzer->shortest_word_length = info->length;
        }
        
        if (info->vowel_count > analyzer->max_vowels) {
            analyzer->max_vowels = info->vowel_count;
            strncpy(analyzer->most_vowels_word, info->word, MAX_WORD_LEN - 1);
            analyzer->most_vowels_word[MAX_WORD_LEN - 1] = '\0';
        }
    }
}

void tokenize_and_process(struct TextProcessor *proc, const char *text) {
    char buffer[512];
    char word[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_pos = 0;
    
    if (proc == NULL || text == NULL) {
        return;
    }
    
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (isalpha(buffer[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = buffer[i];
            }
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                process_word(proc, word);
                word_pos = 0;
            }
        }
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        process_word(proc, word);
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->processor.word_count);
    printf("Total vowels: %d\n