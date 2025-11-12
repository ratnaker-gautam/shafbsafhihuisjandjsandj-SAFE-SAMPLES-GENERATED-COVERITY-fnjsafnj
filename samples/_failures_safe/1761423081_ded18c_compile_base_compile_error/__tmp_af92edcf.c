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

#define PARSE_TIMESTAMP(dest, src) do { \
    if (strlen(src) >= TIMESTAMP_LEN - 1) { \
        strncpy(dest, src, TIMESTAMP_LEN - 1); \
        dest[TIMESTAMP_LEN - 1] = '\0'; \
    } else { \
        strcpy(dest, src); \
    } \
} while(0)

#define PARSE_LEVEL(str, level_ptr) do { \
    if (strncmp(str, "ERROR", 5) == 0) *level_ptr = LEVEL_ERROR; \
    else if (strncmp(str, "WARN", 4) == 0) *level_ptr = LEVEL_WARN; \
    else if (strncmp(str, "INFO", 4) == 0) *level_ptr = LEVEL_INFO; \
    else if (strncmp(str, "DEBUG", 5) == 0) *level_ptr = LEVEL_DEBUG; \
    else *level_ptr = -1; \
} while(0)

void init_stats(struct log_stats *stats) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        stats->counts[i] = 0;
    }
    stats->total = 0;
    stats->first_timestamp[0] = '\0';
    stats->last_timestamp[0] = '\0';
}

int parse_log_line(const char *line, struct log_entry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char *level_str = strtok(NULL, " ");
    if (!level_str) return 0;
    
    char *message = strtok(NULL, "");
    if (!message) return 0;
    
    PARSE_TIMESTAMP(entry->timestamp, timestamp);
    
    int level;
    PARSE_LEVEL(level_str, &level);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    strcpy(entry->message, message);
    
    return 1;
}

void update_stats(struct log_stats *stats, const struct log_entry *entry) {
    if (!stats || !entry) return;
    
    if (IS_VALID_LEVEL(entry->level)) {
        stats->counts[entry->level]++;
        stats->total++;
        
        if (stats->first_timestamp[0] == '\0') {
            strcpy(stats->first_timestamp, entry->timestamp);
        }
        strcpy(stats->last_timestamp, entry->timestamp);
    }
}

void print_stats(const struct log_stats *stats) {
    if (!stats) return;
    
    printf("Log Analysis Summary:\n");
    printf("Time range: %s to %s\n", stats->first_timestamp, stats->last_timestamp);
    printf("Total entries: %d\n", stats->total);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", 
               LEVEL_NAME(i), 
               stats->counts[i],
               stats->total > 0 ? (stats->counts[i] * 100.0 / stats->total) : 0.0);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    init_stats(&stats);
    
    printf("Enter log entries (timestamp LEVEL message), empty line to finish:\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0