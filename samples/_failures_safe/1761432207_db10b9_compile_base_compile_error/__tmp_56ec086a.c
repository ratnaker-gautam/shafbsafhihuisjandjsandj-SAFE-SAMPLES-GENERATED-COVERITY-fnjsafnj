//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

enum log_level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct log_entry {
    time_t timestamp;
    enum log_level level;
    char message[MAX_LINE_LEN];
};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_DEBUG;
    if (strncmp(str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(str, "WARNING", 7) == 0) return LOG_WARNING;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(str, "CRITICAL", 8) == 0) return LOG_CRITICAL;
    
    return -1;
}

int parse_timestamp(const char* str, time_t* timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    
    *timestamp = mktime(&tm);
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len == 0 || len >= MAX_LINE_LEN - 1) continue;
        
        char timestamp_str[32];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp_str, level_str, message) != 3) {
            continue;
        }
        
        time_t timestamp;
        if (!parse_timestamp(timestamp_str, &timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            continue;
        }
        
        entries[count].timestamp = timestamp;
        entries[count].level = level;
        strncpy(entries[count].message, message, MAX_LINE_LEN - 1);
        entries[count].message[MAX_LINE_LEN - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[5] = {0};
    time_t earliest = entries[0].timestamp;
    time_t latest = entries[0].timestamp;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].timestamp < earliest) earliest = entries[i].timestamp;
        if (entries[i].timestamp > latest) latest = entries[i].timestamp;
        
        if (entries[i].level >= 0 && entries[i].level < 5) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %ld seconds\n", (long)(latest - earliest));
    printf("\nBreakdown by level:\n");
    
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d\n", level_names[i], level_counts[i]);
    }
}

void filter_by_level(struct log_entry* entries, int count, enum log_level min_level) {
    if (entries == NULL || count <= 0) return;
    
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    printf("\nEntries with level %s or higher:\n", level_names[min_level]);
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= min_level) {
            char time_buf[32];
            struct tm tm_info;
            localtime_r(&entries[i].timestamp, &tm_info);
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm_info);
            printf("%s %-8s %s\n", time_buf, level_names[entries[i].level], entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries