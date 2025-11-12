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
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' || 
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    int level = PARSE_LEVEL(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct log_stats* stats, const struct log_entry* entry) {
    if (!stats || !entry) return;
    
    stats->counts[entry->level]++;
    stats->total++;
    
    if (stats->first_timestamp[0] == '\0') {
        strncpy(stats->first_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
        stats->first_timestamp[TIMESTAMP_LEN - 1] = '\0';
    }
    
    strncpy(stats->last_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
    stats->last_timestamp[TIMESTAMP_LEN - 1] = '\0';
}

void print_stats(const struct log_stats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Summary:\n");
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("Total entries: %d\n", stats->total);
    printf("Breakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = stats->total > 0 ? (stats->counts[i] * 100.0f) / stats->total : 0.0f;
        printf("  %s: %d (%.1f%%)\n", LEVEL_TO_STR(i), stats->counts[i], percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    INIT_STATS(stats);
    
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line,