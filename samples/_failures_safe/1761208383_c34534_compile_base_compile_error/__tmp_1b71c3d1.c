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

#define LEVEL_NAME(l) (\
    (l) == LEVEL_ERROR ? "ERROR" : \
    (l) == LEVEL_WARN ? "WARN" : \
    (l) == LEVEL_INFO ? "INFO" : \
    (l) == LEVEL_DEBUG ? "DEBUG" : "UNKNOWN")

#define PARSE_LEVEL(str) (\
    strcmp(str, "ERROR") == 0 ? LEVEL_ERROR : \
    strcmp(str, "WARN") == 0 ? LEVEL_WARN : \
    strcmp(str, "INFO") == 0 ? LEVEL_INFO : \
    strcmp(str, "DEBUG") == 0 ? LEVEL_DEBUG : -1)

int parse_log_line(const char* line, struct log_entry* entry) {
    if (!line || !entry) return 0;
    
    char level_str[16];
    int year, month, day, hour, min, sec;
    
    int parsed = sscanf(line, "%d-%d-%d %d:%d:%d %15s %1023[^\n]",
                       &year, &month, &day, &hour, &min, &sec,
                       level_str, entry->message);
    
    if (parsed != 8) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    entry->level = PARSE_LEVEL(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    int written = snprintf(entry->timestamp, TIMESTAMP_LEN, 
                          "%04d-%02d-%02d %02d:%02d:%02d",
                          year, month, day, hour, min, sec);
    if (written < 0 || written >= TIMESTAMP_LEN) return 0;
    
    return 1;
}

void init_stats(struct log_stats* stats) {
    if (!stats) return;
    memset(stats, 0, sizeof(struct log_stats));
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
    
    printf("Log Analysis Summary:\n");
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("Total entries: %d\n", stats->total);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? 
            (100.0 * stats->counts[i]) / stats->total : 0.0;
        printf("%-6s: %d (%.1f%%)\n", LEVEL_NAME(i), stats->counts[i], percentage);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    char line[MAX_LINE_LEN];
    int entry_count = 0;
    
    init_stats(&stats);
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, MAX_LINE_LEN, stdin)) {
        size_t len = strlen(line);
        if (len >