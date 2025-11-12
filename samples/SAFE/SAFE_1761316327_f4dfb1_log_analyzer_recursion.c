//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry, int indent) {
    if (indent < 0 || indent > MAX_LEVELS) return;
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int max_level) {
    if (start >= end || current_level > max_level) return;
    if (start < 0 || end < 0 || current_level < 0) return;
    
    int level_count[5] = {0};
    
    for (int i = start; i < end; i++) {
        int level = parse_log_level(entries[i].level);
        if (level >= 0 && level < 5) {
            level_count[level]++;
        }
    }
    
    printf("Level %d analysis (%d-%d): ", current_level, start, end);
    printf("DEBUG:%d INFO:%d WARNING:%d ERROR:%d CRITICAL:%d\n",
           level_count[0], level_count[1], level_count[2], 
           level_count[3], level_count[4]);
    
    if (current_level < max_level) {
        int mid = start + (end - start) / 2;
        if (mid > start && mid < end) {
            analyze_log_recursive(entries, start, mid, current_level + 1, max_level);
            analyze_log_recursive(entries, mid, end, current_level + 1, max_level);
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    const char* sample_logs[] = {
        "INFO 1000 System startup completed",
        "DEBUG 1001 Initializing modules",
        "WARNING 1002 High memory usage detected",
        "ERROR 1003 Database connection failed",
        "INFO 1004 User login successful",
        "DEBUG 1005 Cache populated",
        "CRITICAL 1006 System crash imminent",
        "INFO 1007 Backup initiated",
        "WARNING 1008 Disk space low",
        "ERROR 1009 Network timeout"
    };
    
    int num_samples = sizeof(sample_logs) / sizeof(sample_logs[0]);
    int entries_read = 0;
    
    for (int i = 0; i < num_samples && entries_read < max_entries; i++) {
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int result = sscanf(sample_logs[i], "%15s %d %1023[^\n]", 
                           level, &timestamp, message);
        if (result == 3) {
            strncpy(entries[entries_read].level, level, 15);
            entries[entries_read].level[15] = '\0';
            entries[entries_read].timestamp = timestamp;
            strncpy(entries[entries_read].message, message, MAX_LINE_LENGTH - 1);
            entries[entries_read].message[MAX_LINE_LENGTH - 1] = '\0';
            entries_read++;
        }
    }
    
    return entries_read;
}

int main(void) {
    struct LogEntry entries[20];
    int num_entries = read_log_entries(entries, 20);
    
    if (num_entries <= 0) {
        printf("No log entries to analyze\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("=====================\n\n");
    
    for (int i = 0; i < num_entries; i++) {
        print_log_entry(&entries[i], 0);
    }
    
    printf("\nStatistical Analysis:\n");
    analyze_log_recursive(entries, 0, num_entries, 0, 3);
    
    return 0;
}