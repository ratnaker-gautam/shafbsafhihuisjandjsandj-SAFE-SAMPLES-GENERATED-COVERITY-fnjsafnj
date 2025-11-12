//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_text_stats(struct TextStats *stats) {
    stats->word_count = 0;
    stats->total_vowels = 0;
    stats->total_consonants = 0;
    stats->longest_word_length = 0;
    stats->shortest_word_length = MAX_WORD_LEN;
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

int process_text(const char *text, struct TextStats *stats) {
    if (text == NULL || stats == NULL) return 0;
    
    initialize_text_stats(stats);
    
    char buffer[MAX_WORD_LEN];
    int buffer_index = 0;
    size_t text_len = strlen(text);
    
    for (size_t i = 0; i <= text_len; i++) {
        if (i < text_len && isalpha(text[i])) {
            if (buffer_index < MAX_WORD_LEN - 1) {
                buffer[buffer_index++] = text[i];
            }
        } else {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                
                if (stats->word_count < MAX_WORDS) {
                    struct WordInfo *current = &stats->words[stats->word_count];
                    if (analyze_word(buffer, current)) {
                        stats->total_vowels += current->vowel_count;
                        stats->total_consonants += current->consonant_count;
                        
                        if (current->length > stats->longest_word_length) {
                            stats->longest_word_length = current->length;
                        }
                        if (current->length < stats->shortest_word_length) {
                            stats->shortest_word_length = current->length;
                        }
                        
                        stats->word_count++;
                    }
                }
                buffer_index = 0;
            }
        }
    }
    
    return stats->word_count > 0;
}

void print_stats(const struct TextStats *stats) {
    if (stats == NULL || stats->word_count == 0) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", stats->word_count);
    printf("Total vowels: %d\n", stats->total_vowels);
    printf("Total consonants: %d\n", stats->total_consonants);
    printf("Longest word length: %d\n", stats->longest_word_length);
    printf("Shortest word length: %d\n", stats->shortest_word_length);
    
    printf("\nWord details:\n");
    for (int i = 0; i < stats->word_count; i++) {
        const struct WordInfo *word = &stats->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    char input_text[1024];
    struct TextStats stats;
    
    printf("Enter text to analyze: ");
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_text(input_text, &stats)) {
        printf("No valid words found in the text.\n");
        return 1;
    }
    
    print_stats(&stats);
    
    return 0;
}