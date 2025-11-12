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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcmp(level_str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcmp(level_str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

const char* level_to_string(int level) {
    static const char* levels[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    if (IS_VALID_LEVEL(level)) return levels[level];
    return "UNKNOWN";
}

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    if (strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) == NULL) return 0;
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        int parsed = sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) continue;
        
        if (!parse_timestamp(timestamp)) continue;
        
        int level = parse_log_level(level_str);
        if (level == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
        
        if (i == 0) {
            strcpy(earliest, entries[i].timestamp);
            strcpy(latest, entries[i].timestamp);
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strcpy(earliest, entries[i].timestamp);
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strcpy(latest, entries[i].timestamp);
            }
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Level distribution:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d\n", level_to_string(i), level_counts[i]);
    }
}

void filter_by_level(struct log_entry* entries, int count, int min_level) {
    if (!IS_VALID_LEVEL(min_level)) return;
    
    printf("\nEntries with level %s and above:\n", level_to_string(min_level));
    for (int i = 0; i < count; i++) {
        if (entries[i].level <= min_level) {
            printf("%s %s %s\n", entries[i].timestamp, level_to_string(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    printf("\nEnter minimum level to filter (0=ERROR, 1=