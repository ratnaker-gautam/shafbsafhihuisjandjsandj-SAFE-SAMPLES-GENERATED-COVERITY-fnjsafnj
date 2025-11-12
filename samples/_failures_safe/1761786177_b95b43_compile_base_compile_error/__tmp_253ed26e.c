//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LEVEL_ERROR 0
#define LEVEL_WARN 1
#define LEVEL_INFO 2
#define LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LEVEL_ERROR && (l) <= LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

struct log_stats {
    int counts[LOG_LEVELS];
    int total;
    char first_timestamp[TIMESTAMP_LEN];
    char last_timestamp[TIMESTAMP_LEN];
};

#define INIT_STATS(s) do { \
    memset((s).counts, 0, sizeof((s).counts)); \
    (s).total = 0; \
    (s).first_timestamp[0] = '\0'; \
    (s).last_timestamp[0] = '\0'; \
} while(0)

#define PARSE_LEVEL(str) ( \
    strncmp((str), "ERROR", 5) == 0 ? LEVEL_ERROR : \
    strncmp((str), "WARN", 4) == 0 ? LEVEL_WARN : \
    strncmp((str), "INFO", 4) == 0 ? LEVEL_INFO : \
    strncmp((str), "DEBUG", 5) == 0 ? LEVEL_DEBUG : -1)

#define LEVEL_TO_STR(l) ( \
    (l) == LEVEL_ERROR ? "ERROR" : \
    (l) == LEVEL_WARN ? "WARN" : \
    (l) == LEVEL_INFO ? "INFO" : \
    (l) == LEVEL_DEBUG ? "DEBUG" : "UNKNOWN")

int parse_log_line(const char* line, struct log_entry* entry) {
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    const char* timestamp_start = line;
    const char* level_start = strchr(line, ' ');
    if (!level_start) return 0;
    
    size_t timestamp_len = level_start - timestamp_start;
    if (timestamp_len >= TIMESTAMP_LEN || timestamp_len < 10) return 0;
    
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    for (size_t i = 0; i < timestamp_len; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.' && 
            entry->timestamp[i] != ' ') return 0;
    }
    
    level_start++;
    const char* message_start = strchr(level_start, ' ');
    if (!message_start) return 0;
    
    size_t level_len = message_start - level_start;
    if (level_len > 10) return 0;
    
    char level_str[11];
    strncpy(level_str, level_start, level_len);
    level_str[level_len] = '\0';
    
    entry->level = PARSE_LEVEL(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    message_start++;
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

void update_stats(struct log_stats* stats, const struct log_entry* entry) {
    if (!stats || !entry) return;
    
    if (stats->total == 0) {
        strncpy(stats->first_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
        stats->first_timestamp[TIMESTAMP_LEN - 1] = '\0';
    }
    
    strncpy(stats->last_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
    stats->last_timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
    }
    stats->total++;
}

void print_stats(const struct log_stats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("Level distribution:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? 
            (100.0 * stats->counts[i]) / stats->total : 0.0;
        printf("  %s: %d (%.1f%%)\n", LEVEL_TO_STR(i), stats->counts[i], percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    INIT_STATS(stats);
    
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf