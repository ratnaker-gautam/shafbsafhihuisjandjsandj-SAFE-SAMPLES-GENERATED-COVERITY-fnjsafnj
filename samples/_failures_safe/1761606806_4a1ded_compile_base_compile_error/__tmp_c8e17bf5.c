//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(ts_str, ts_struct) \
    do { \
        if (strptime(ts_str, "%Y-%m-%d %H:%M:%S", &(ts_struct)) == NULL) { \
            return -1; \
        } \
    } while(0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

typedef struct {
    int total;
    int level_counts[LOG_LEVELS];
    time_t start_time;
    time_t end_time;
} LogStats;

const char* level_strings[LOG_LEVELS] = {"INFO", "WARN", "ERROR", "DEBUG"};

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL || strlen(line) >= MAX_LINE_LEN) {
        return -1;
    }
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return -1;
    }
    
    if (strlen(timestamp) != TIMESTAMP_LEN - 1 || !IS_VALID_LEVEL(level)) {
        return -1;
    }
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, 7);
    entry->level[7] = '\0';
    strncpy(entry->message, message, MAX_LINE_LEN - TIMESTAMP_LEN - 10);
    entry->message[MAX_LINE_LEN - TIMESTAMP_LEN - 11] = '\0';
    
    return 0;
}

int get_level_index(const char* level) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(level, level_strings[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void update_stats(LogStats* stats, const LogEntry* entry) {
    if (stats == NULL || entry == NULL) return;
    
    stats->total++;
    
    int level_idx = get_level_index(entry->level);
    if (level_idx >= 0 && level_idx < LOG_LEVELS) {
        stats->level_counts[level_idx]++;
    }
    
    struct tm tm_time = {0};
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm_time) != NULL) {
        time_t entry_time = mktime(&tm_time);
        if (stats->total == 1) {
            stats->start_time = entry_time;
            stats->end_time = entry_time;
        } else {
            if (entry_time < stats->start_time) stats->start_time = entry_time;
            if (entry_time > stats->end_time) stats->end_time = entry_time;
        }
    }
}

void print_stats(const LogStats* stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: ");
    
    if (stats->total > 0) {
        char start_str[TIMESTAMP_LEN], end_str[TIMESTAMP_LEN];
        struct tm* tm_start = localtime(&stats->start_time);
        struct tm* tm_end = localtime(&stats->end_time);
        
        if (tm_start && tm_end) {
            strftime(start_str, TIMESTAMP_LEN, "%Y-%m-%d %H:%M:%S", tm_start);
            strftime(end_str, TIMESTAMP_LEN, "%Y-%m-%d %H:%M:%S", tm_end);
            printf("%s to %s\n", start_str, end_str);
        } else {
            printf("Unknown\n");
        }
    } else {
        printf("No entries\n");
    }
    
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = (stats->total > 0) ? 
            (100.0 * stats->level_counts[i] / stats->total) : 0.0;
        printf("  %s: %d (%.1f%%)\n", level_strings[i], 
               stats->level_counts[i], percentage);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    LogStats stats = {0};
    int entry