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

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

struct log_stats {
    int counts[LOG_LEVELS];
    int total;
    char start_time[TIMESTAMP_LEN];
    char end_time[TIMESTAMP_LEN];
};

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

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
    
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        if (len == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) < 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        struct log_entry* entry = &entries[count];
        
        if (!parse_timestamp(timestamp, entry->timestamp)) {
            printf("Invalid timestamp. Skipping.\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            printf("Invalid log level. Skipping.\n");
            continue;
        }
        
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(const struct log_entry* entries, int count, struct log_stats* stats) {
    if (!entries || !stats || count <= 0) {
        memset(stats, 0, sizeof(*stats));
        return;
    }
    
    memset(stats, 0, sizeof(*stats));
    stats->total = count;
    
    for (int i = 0; i < count; i++) {
        const struct log_entry* entry = &entries[i];
        if (IS_VALID_LEVEL(entry->level)) {
            stats->counts[entry->level]++;
        }
    }
    
    if (count > 0) {
        strncpy(stats->start_time, entries[0].timestamp, TIMESTAMP_LEN - 1);
        stats->start_time[TIMESTAMP_LEN - 1] = '\0';
        strncpy(stats->end_time, entries[count-1].timestamp, TIMESTAMP_LEN - 1);
        stats->end_time[TIMESTAMP_LEN - 1