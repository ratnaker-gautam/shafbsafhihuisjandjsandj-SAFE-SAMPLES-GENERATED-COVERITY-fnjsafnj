//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][64], int max_words) {
    if (!is_valid_input(input) || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        unsigned char c = (i < input_len) ? (unsigned char)input[i] : ' ';
        
        if (isalnum(c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 64) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

void count_word_frequency(char words[][64], int word_count, struct WordCount *freq, int *freq_count) {
    if (words == NULL || freq == NULL || freq_count == NULL || word_count <= 0) return;
    
    *freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *freq_count; j++) {
            if (strcmp(words[i], freq[j].word) == 0) {
                freq[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && *freq_count < MAX_WORDS) {
            strncpy(freq[*freq_count].word, words[i], 63);
            freq[*freq_count].word[63] = '\0';
            freq[*freq_count].count = 1;
            (*freq_count)++;
        }
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void process_input_string(const char *input) {
    if (!is_valid_input(input)) {
        printf("Invalid input string\n");
        return;
    }
    
    char words[MAX_WORDS][64];
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    struct WordCount freq[MAX_WORDS];
    int freq_count = 0;
    count_word_frequency(words, word_count, freq, &freq_count);
    
    qsort(freq, freq_count, sizeof(struct WordCount), compare_word_count);
    
    printf("Word frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", freq_count);
    printf("\nTop words:\n");
    
    int display_count = (freq_count < 10) ? freq_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%d. '%s' - %d occurrences\n", i + 1, freq[i].word, freq[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input_string(input);
    
    return 0;
}