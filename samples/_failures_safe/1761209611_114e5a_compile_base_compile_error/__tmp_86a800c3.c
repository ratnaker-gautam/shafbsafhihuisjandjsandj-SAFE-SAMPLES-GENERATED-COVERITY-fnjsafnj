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
#define LEVEL_WARN  1
#define LEVEL_INFO  2
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

#define LEVEL_STR(l) (\
    (l) == LEVEL_ERROR ? "ERROR" : \
    (l) == LEVEL_WARN  ? "WARN"  : \
    (l) == LEVEL_INFO  ? "INFO"  : \
    "DEBUG"\
)

#define PARSE_TIMESTAMP(dest, src) do { \
    if (strlen(src) >= TIMESTAMP_LEN-1) { \
        strncpy(dest, src, TIMESTAMP_LEN-1); \
        dest[TIMESTAMP_LEN-1] = '\0'; \
    } else { \
        strcpy(dest, src); \
    } \
} while(0)

int parse_log_level(const char* str) {
    if (!str) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (!line || !entry) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %9s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    PARSE_TIMESTAMP(entry->timestamp, timestamp);
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->message[sizeof(entry->message)-1] = '\0';
    
    return 1;
}

void init_stats(struct log_stats* stats) {
    if (!stats) return;
    memset(stats, 0, sizeof(*stats));
    for (int i = 0; i < LOG_LEVELS; i++) {
        stats->counts[i] = 0;
    }
    stats->total = 0;
    stats->first_timestamp[0] = '\0';
    stats->last_timestamp[0] = '\0';
}

void update_stats(struct log_stats* stats, const struct log_entry* entry) {
    if (!stats || !entry) return;
    
    if (stats->total == 0) {
        strncpy(stats->first_timestamp, entry->timestamp, TIMESTAMP_LEN-1);
        stats->first_timestamp[TIMESTAMP_LEN-1] = '\0';
    }
    strncpy(stats->last_timestamp, entry->timestamp, TIMESTAMP_LEN-1);
    stats->last_timestamp[TIMESTAMP_LEN-1] = '\0';
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
    }
    stats->total++;
}

void print_stats(const struct log_stats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? (stats->counts[i] * 100.0) / stats->total : 0.0;
        printf("%-6s: %d (%.1f%%)\n", LEVEL_STR(i), stats->counts[i], percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    init_stats(&stats);
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input.\n\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-