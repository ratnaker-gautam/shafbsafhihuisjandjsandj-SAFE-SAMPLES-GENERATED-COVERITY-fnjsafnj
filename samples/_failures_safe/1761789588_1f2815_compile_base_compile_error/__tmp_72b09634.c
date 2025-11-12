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

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp || strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char *str) {
    if (!str) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int read_log_entries(struct log_entry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    if (!entries || max_entries <= 0) return 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        size_t len = strlen(line);
        if (len == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char *level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ' && level_start < line + sizeof(line) - 1) {
            level_start++;
        }
        
        int level = parse_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char *message_start = level_start;
        while (*message_start && *message_start != ' ' && *message_start != '\t') {
            message_start++;
        }
        while (*message_start == ' ' || *message_start == '\t') {
            message_start++;
        }
        
        if (strlen(message_start) == 0) continue;
        
        strcpy(entries[count].timestamp, timestamp);
        entries[count].level = level;
        strncpy(entries[count].message, message_start, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        char *newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void calculate_stats(const struct log_entry *entries, int count, struct log_stats *stats) {
    if (!entries || !stats || count < 0) return;
    
    memset(stats, 0, sizeof(*stats));
    stats->total = count;
    
    if (count == 0) return;
    
    strcpy(stats->first_timestamp, entries[0].timestamp);
    strcpy(stats->last_timestamp, entries[count - 1].timestamp);
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            stats->counts[entries[i].level]++;
        }
    }
}

void print_stats(const struct log_stats *stats) {
    if (!stats) return;
    
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries: %d\n", stats->total);
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        float percentage = stats->total > 0 ? (stats->counts[i] * 100.0f) / stats->total : 0.0f;
        printf("%-6s: %d (%.1f%%)\