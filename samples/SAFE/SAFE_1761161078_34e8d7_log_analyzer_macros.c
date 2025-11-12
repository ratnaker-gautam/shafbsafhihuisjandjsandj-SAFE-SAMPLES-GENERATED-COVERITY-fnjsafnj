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

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)
#define IS_VALID_TIMESTAMP(ts) (strlen(ts) == TIMESTAMP_LEN - 1)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char* level_str) {
    if (!level_str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(level_str, level_names[i]) == 0) return i;
    }
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (!timestamp || !IS_VALID_TIMESTAMP(timestamp)) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN];
        
        int parsed = sscanf(line, "%19s %15s %1023[^\n]", 
                           timestamp, level_str, message);
        
        if (parsed != 3) continue;
        
        if (!validate_timestamp(timestamp)) continue;
        
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

void analyze_logs(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    int total_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
            total_entries++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = total_entries > 0 ? 
            (level_counts[i] * 100.0) / total_entries : 0.0;
        printf("%-8s: %d (%.1f%%)\n", 
               level_names[i], level_counts[i], percentage);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, level_names[entries[0].level], entries[0].message);
        printf("Last entry:  %s %s %s\n", 
               entries[count-1].timestamp, level_names[entries[count-1].level], entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count >= MAX_ENTRIES) {
        printf("Warning: Reached maximum entry limit (%d)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}