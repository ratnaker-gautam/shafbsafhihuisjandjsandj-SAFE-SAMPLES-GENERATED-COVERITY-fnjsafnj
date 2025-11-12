//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
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

int find_word_in_stats(struct WordStats *stats, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(stats[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordStats *stats, int *word_count) {
    if (input == NULL || stats == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    to_lower_case(buffer);
    
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN];
    int stats_count = *word_count;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (is_valid_char(buffer[i])) {
            if (current_word_len < MAX_WORD_LEN - 1) {
                current_word[current_word_len++] = buffer[i];
            }
        } else {
            if (current_word_len > 0) {
                current_word[current_word_len] = '\0';
                
                int found_index = find_word_in_stats(stats, stats_count, current_word);
                if (found_index >= 0) {
                    stats[found_index].count++;
                } else if (stats_count < MAX_WORD_COUNT) {
                    if (safe_strncpy(stats[stats_count].word, current_word, MAX_WORD_LEN)) {
                        stats[stats_count].count = 1;
                        stats[stats_count].length = current_word_len;
                        stats_count++;
                    }
                }
                current_word_len = 0;
            }
        }
    }
    
    if (current_word_len > 0) {
        current_word[current_word_len] = '\0';
        int found_index = find_word_in_stats(stats, stats_count, current_word);
        if (found_index >= 0) {
            stats[found_index].count++;
        } else if (stats_count < MAX_WORD_COUNT) {
            if (safe_strncpy(stats[stats_count].word, current_word, MAX_WORD_LEN)) {
                stats[stats_count].count = 1;
                stats[stats_count].length = current_word_len;
                stats_count++;
            }
        }
    }
    
    *word_count = stats_count;
    return 1;
}

void print_word_stats(const struct WordStats *stats, int count) {
    if (stats == NULL || count <= 0) return;
    
    printf("\nWord Statistics:\n");
    printf("================\n");
    
    for (int i = 0; i < count; i++) {
        printf("Word: %-15s Count: %3d Length: %2d\n", 
               stats[i].word, stats[i].count, stats[i].length);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordStats word_stats[MAX_WORD_COUNT];
    int word_count = 0;
    
    printf("Text Word Frequency Analyzer\n");
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    printf("> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_input(input, word_stats, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_word_stats(word_stats, word_count);
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += word_stats[i].count;
    }
    
    printf("\nSummary: %d unique words, %d total words\n", word_count, total_words);
    
    return 0;
}