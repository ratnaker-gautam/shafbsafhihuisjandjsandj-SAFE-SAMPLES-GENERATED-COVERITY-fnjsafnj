//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordStats {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
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

int analyze_word(const char *word, struct WordStats *stats) {
    if (word == NULL || stats == NULL) return 0;
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(stats->word, word, MAX_WORD_LEN - 1);
    stats->word[MAX_WORD_LEN - 1] = '\0';
    stats->length = (int)len;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; i < stats->length; i++) {
        if (isalpha(stats->word[i])) {
            if (is_vowel(stats->word[i])) {
                stats->vowel_count++;
            } else {
                stats->consonant_count++;
            }
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
        if (isalnum(*ptr)) {
            if (buffer_index < MAX_WORD_LEN - 1) {
                buffer[buffer_index++] = *ptr;
            }
        } else {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                
                if (analyze_word(buffer, &analyzer->words[analyzer->word_count])) {
                    struct WordStats *current = &analyzer->words[analyzer->word_count];
                    
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
        if (analyze_word(buffer, &analyzer->words[analyzer->word_count])) {
            struct WordStats *current = &analyzer->words[analyzer->word_count];
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
    if (analyzer == NULL || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    char input_text[1024];
    
    printf("Enter text to analyze: ");
    if (fgets(input_text, sizeof(input_text), stdin