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
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
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

int process_text(const char *input, struct TextStats *stats) {
    char buffer[512];
    char *token;
    char *saveptr;
    
    if (strlen(input) >= sizeof(buffer)) {
        return 0;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    token = strtok_r(buffer, " \t\n\r", &saveptr);
    while (token != NULL && stats->word_count < MAX_WORDS) {
        if (analyze_word(token, &stats->words[stats->word_count])) {
            struct WordInfo *current = &stats->words[stats->word_count];
            
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
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    
    return 1;
}

void print_stats(const struct TextStats *stats) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", stats->word_count);
    printf("Total vowels: %d\n", stats->total_vowels);
    printf("Total consonants: %d\n", stats->total_consonants);
    printf("Longest word length: %d\n", stats->longest_word_length);
    printf("Shortest word length: %d\n", stats->shortest_word_length);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < stats->word_count; i++) {
        const struct WordInfo *word = &stats->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    struct TextStats stats;
    char input[512];
    
    initialize_text_stats(&stats);
    
    printf("Enter text to analyze (max 511 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!process_text(input, &stats)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (stats.word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    print_stats(&stats);
    
    return 0;
}