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
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
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
    time_t start_time;
    time_t end_time;
};

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_timestamp(const char* str, time_t* result) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *result = mktime(&tm);
    return *result != -1;
}

int parse_log_level(const char* str) {
    if (strcmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int read_log_entries(struct log_entry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        int parsed = sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) continue;
        
        time_t entry_time;
        if (!parse_timestamp(timestamp, &entry_time)) continue;
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void calculate_stats(const struct log_entry entries[], int count, struct log_stats* stats) {
    memset(stats, 0, sizeof(*stats));
    
    if (count == 0) return;
    
    time_t min_time = 0, max_time = 0;
    for (int i = 0; i < count; i++) {
        time_t entry_time;
        if (parse_timestamp(entries[i].timestamp, &entry_time)) {
            if (i == 0 || entry_time < min_time) min_time = entry_time;
            if (i == 0 || entry_time > max_time) max_time = entry_time;
        }
        
        if (IS_VALID_LEVEL(entries[i].level)) {
            stats->counts[entries[i].level]++;
            stats->total++;
        }
    }
    
    stats->start_time = min_time;
    stats->end_time = max_time;
}

void print_stats(const struct log_stats* stats) {
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries: %d\n", stats->total);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = stats->total > 0 ? (stats->counts[i] * 100.0) / stats->total : 0.0;
        printf("%-6s: %d (%.1f%%)\n", level_names[i], stats->counts[i], percentage);
    }
    
    if (stats->start_time != 0 && stats->end_time != 0) {
        double duration = difftime(stats->end_time, stats->start_time) / 3600.0;
        printf("Time span: %.2f hours\n", duration);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    struct log_stats stats;
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Press Ctrl+D to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    calculate_stats