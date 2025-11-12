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

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6)
        return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    snprintf(timestamp, TIMESTAMP_LEN, "%04d-%02d-%02d %02d:%02d:%02d", 
             year, month, day, hour, min, sec);
    return 1;
}

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
    
    char timestamp_buf[TIMESTAMP_LEN];
    char level_buf[16];
    char message_buf[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %1023[^\n]", 
               timestamp_buf, level_buf, message_buf) != 3)
        return 0;
    
    if (!parse_timestamp(timestamp_buf, entry->timestamp))
        return 0;
    
    int level = parse_log_level(level_buf);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    entry->level = level;
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
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
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
        stats->total++;
        
        if (stats->first_timestamp[0] == '\0') {
            strncpy(stats->first_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
            stats->first_timestamp[TIMESTAMP_LEN - 1] = '\0';
        }
        
        strncpy(stats->last_timestamp, entry->timestamp, TIMESTAMP_LEN - 1);
        stats->last_timestamp[TIMESTAMP_LEN - 1] = '\0';
    }
}

void print_stats(const struct log_stats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? 
            (100.0 * stats->counts[i]) / stats->total : 0.0;
        printf("%-6s: %d (%.1f%%)\n", level_names[i], stats->count