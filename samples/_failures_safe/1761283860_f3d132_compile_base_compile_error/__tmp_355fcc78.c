//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->word_count = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int analyze_word(const char *word, struct WordInfo *info) {
    if (word == NULL || info == NULL) return 0;
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    info->length = (int)len;
    info->vowel_count = 0;
    info->consonant_count = 0;
    
    for (int i = 0; i < info->length; i++) {
        if (is_vowel(info->word[i])) {
            info->vowel_count++;
        } else if (is_consonant(info->word[i])) {
            info->consonant_count++;
        }
    }
    
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return 0;
    
    initialize_analyzer(analyzer);
    
    char buffer[MAX_WORD_LEN];
    const char *ptr = text;
    int buffer_index = 0;
    
    while (*ptr != '\0' && analyzer->word_count < MAX_WORDS) {
        if (isalpha(*ptr)) {
            if (buffer_index < MAX_WORD_LEN - 1) {
                buffer[buffer_index++] = *ptr;
            }
        } else {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                struct WordInfo *current = &analyzer->words[analyzer->word_count];
                
                if (analyze_word(buffer, current)) {
                    analyzer->total_vowels += current->vowel_count;
                    analyzer->total_consonants += current->consonant_count;
                    
                    if (current->length > analyzer->longest_word_length) {
                        analyzer->longest_word_length = current->length;
                    }
                    if (current->length < analyzer->shortest_word_length) {
                        analyzer->shortest_word_length = current->length;
                    }
                    
                    analyzer->word_count++;
                }
                buffer_index = 0;
            }
        }
        ptr++;
    }
    
    if (buffer_index > 0 && analyzer->word_count < MAX_WORDS) {
        buffer[buffer_index] = '\0';
        struct WordInfo *current = &analyzer->words[analyzer->word_count];
        
        if (analyze_word(buffer, current)) {
            analyzer->total_vowels += current->vowel_count;
            analyzer->total_consonants += current->consonant_count;
            
            if (current->length > analyzer->longest_word_length) {
                analyzer->longest_word_length = current->length;
            }
            if (current->length < analyzer->shortest_word_length) {
                analyzer->shortest_word_length = current->length;
            }
            
            analyzer->word_count++;
        }
    }
    
    return analyzer->word_count > 0;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    
    if (analyzer->word_count > 0) {
        printf("Longest word length: %d\n", analyzer->longest_word_length);
        printf("Shortest word length: %d\n", analyzer->shortest_word_length);
        printf("Average word length: %.2f\n", 
               (float)(analyzer->total_vowels + analyzer->total_consonants) / analyzer->word_count);
        
        printf("\nWord details:\n");
        for (int i = 0; i < analyzer->word_count; i++) {
            printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
                   analyzer->words[i].word,
                   analyzer->words[i].length,
                   analyzer->words[i].vowel_count,
                   analyzer->words[i