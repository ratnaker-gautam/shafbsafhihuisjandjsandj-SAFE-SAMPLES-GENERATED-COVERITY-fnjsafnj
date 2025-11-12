//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogStats {
    int error_count;
    int warning_count;
    int info_count;
    int total_entries;
    char most_frequent_level[16];
    int max_frequency;
};

void parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp && level && message) {
        strncpy(entry->timestamp, timestamp, 31);
        entry->timestamp[31] = '\0';
        
        strncpy(entry->level, level, 15);
        entry->level[15] = '\0';
        
        strncpy(entry->message, message, 255);
        entry->message[255] = '\0';
        
        if (strcmp(level, "ERROR") == 0) entry->severity = 3;
        else if (strcmp(level, "WARNING") == 0) entry->severity = 2;
        else if (strcmp(level, "INFO") == 0) entry->severity = 1;
        else entry->severity = 0;
    }
}

void analyze_logs(struct LogEntry* entries, int count, struct LogStats* stats) {
    if (!entries || !stats || count <= 0) return;
    
    memset(stats, 0, sizeof(struct LogStats));
    stats->total_entries = count;
    
    int level_counts[3] = {0};
    char* level_names[] = {"INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 1: stats->info_count++; level_counts[0]++; break;
            case 2: stats->warning_count++; level_counts[1]++; break;
            case 3: stats->error_count++; level_counts[2]++; break;
        }
    }
    
    int max_idx = 0;
    for (int i = 1; i < 3; i++) {
        if (level_counts[i] > level_counts[max_idx]) {
            max_idx = i;
        }
    }
    
    strncpy(stats->most_frequent_level, level_names[max_idx], 15);
    stats->most_frequent_level[15] = '\0';
    stats->max_frequency = level_counts[max_idx];
}

void generate_sample_logs(struct LogEntry** entries, int* count) {
    if (!entries || !count) return;
    
    *count = 50;
    *entries = malloc(*count * sizeof(struct LogEntry));
    if (!*entries) {
        *count = 0;
        return;
    }
    
    char* levels[] = {"INFO", "WARNING", "ERROR"};
    char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "Memory usage high",
        "Disk space running low",
        "Network timeout occurred",
        "Critical error in module",
        "Backup completed",
        "Security scan finished",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    
    for (int i = 0; i < *count; i++) {
        struct tm* tm_info = localtime(&now);
        strftime((*entries)[i].timestamp, 31, "%Y-%m-%d %H:%M:%S", tm_info);
        
        int level_idx = rand() % 3;
        strncpy((*entries)[i].level, levels[level_idx], 15);
        (*entries)[i].level[15] = '\0';
        
        int msg_idx = rand() % 10;
        strncpy((*entries)[i].message, messages[msg_idx], 255);
        (*entries)[i].message[255] = '\0';
        
        (*entries)[i].severity = level_idx + 1;
        
        now += 60;
    }
}

void print_analysis(const struct LogStats* stats) {
    if (!stats) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s (%d occurrences)\n", 
           stats->most_frequent_level, stats->max_frequency);
    
    if (stats->total_entries > 0) {
        printf("Distribution:\n");
        printf("  INFO: %.1f%%\n", (stats->info_count * 100.0) / stats->total_entries);
        printf("  WARNING: %.1f%%\n", (stats->