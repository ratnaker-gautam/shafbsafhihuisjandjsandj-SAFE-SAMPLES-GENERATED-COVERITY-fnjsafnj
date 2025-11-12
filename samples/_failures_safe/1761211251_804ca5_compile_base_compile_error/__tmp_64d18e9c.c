//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(level) ((level) >= LOG_LEVEL_ERROR && (level) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

struct log_stats {
    int counts[LOG_LEVELS];
    int total;
    time_t start_time;
    time_t end_time;
};

#define INIT_STATS(stats) do { \
    memset((stats).counts, 0, sizeof((stats).counts)); \
    (stats).total = 0; \
    (stats).start_time = 0; \
    (stats).end_time = 0; \
} while(0)

#define PARSE_TIMESTAMP(str, tm) do { \
    if (strptime((str), "%Y-%m-%d %H:%M:%S", (tm)) == NULL) { \
        if (strptime((str), "%Y/%m/%d %H:%M:%S", (tm)) == NULL) { \
            (tm)->tm_year = 0; \
        } \
    } \
} while(0)

int parse_log_level(const char* level_str) {
    if (level_str == NULL) return -1;
    if (strncmp(level_str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(level_str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(level_str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(level_str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

const char* level_to_string(int level) {
    static const char* levels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    if (IS_VALID_LEVEL(level)) return levels[level];
    return "UNKNOWN";
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[TIMESTAMP_LEN] = {0};
    char level_str[16] = {0};
    char message[MAX_LINE_LEN] = {0};
    
    int parsed = sscanf(line, "%19s %15s %1023[^\n]", 
                       timestamp, level_str, message);
    
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) != 19) return 0;
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct log_entry* entries, int count, struct log_stats* stats) {
    if (entries == NULL || stats == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    INIT_STATS(*stats);
    
    struct tm tm_start = {0}, tm_end = {0};
    time_t current_time;
    
    for (int i = 0; i < count; i++) {
        if (!IS_VALID_LEVEL(entries[i].level)) continue;
        
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            stats->counts[entries[i].level]++;
        }
        stats->total++;
        
        PARSE_TIMESTAMP(entries[i].timestamp, &tm_start);
        if (tm_start.tm_year != 0) {
            current_time = mktime(&tm_start);
            if (current_time != (time_t)-1) {
                if (stats->start_time == 0 || current_time < stats->start_time) {
                    stats->start_time = current_time;
                }
                if (current_time > stats->end_time) {
                    stats->end_time = current_time;
                }
            }
        }
    }
}

void print_statistics(const struct log_stats* stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = stats->total > 0 ? 
            (stats->counts[i] * 100.0f) / stats->total : 0.0f;
        printf("%-6s: %d (%.1f%%)\n", 
               level_to_string(i), stats->counts[i], percentage