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

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char* timestamp_start = line;
        while (*timestamp_start && isspace(*timestamp_start)) timestamp_start++;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(timestamp_start, timestamp)) continue;
        
        char* level_start = strchr(timestamp_start, ']');
        if (!level_start) continue;
        level_start++;
        while (*level_start && isspace(*level_start)) level_start++;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        char* message_start = level_start;
        while (*message_start && !isspace(*message_start)) message_start++;
        while (*message_start && isspace(*message_start)) message_start++;
        
        if (strlen(message_start) == 0) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message_start, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        char* newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void calculate_stats(const struct log_entry* entries, int count, struct log_stats* stats) {
    if (!entries || !stats || count <= 0) {
        memset(stats, 0, sizeof(*stats));
        return;
    }
    
    memset(stats, 0, sizeof(*stats));
    stats->total = count;
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            stats->counts[entries[i].level]++;
        }
        
        if (i == 0) {
            strncpy(stats->first_timestamp, entries[i].timestamp, TIMESTAMP_LEN - 1);
            stats->first_timestamp[TIMESTAMP_LEN - 1] = '\0';
        }
        if (i == count - 1) {
            strncpy(stats->last_timestamp, entries[i].timestamp, TIMESTAMP_LEN - 1);
            stats->last_timestamp[TIMESTAMP_LEN - 1] = '\0';