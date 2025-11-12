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
    int max_level_count;
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
    
    int level_counts[4] = {0};
    char* level_names[] = {"UNKNOWN", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: stats->error_count++; level_counts[3]++; break;
            case 2: stats->warning_count++; level_counts[2]++; break;
            case 1: stats->info_count++; level_counts[1]++; break;
            default: level_counts[0]++; break;
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    stats->max_level_count = max_count;
    strncpy(stats->most_frequent_level, level_names[max_index], 15);
    stats->most_frequent_level[15] = '\0';
}

void generate_sample_logs(struct LogEntry* entries, int* count) {
    if (!entries || !count || *count <= 0) return;
    
    char* levels[] = {"INFO", "WARNING", "ERROR"};
    char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Failed to connect to service",
        "Backup completed",
        "Security alert triggered",
        "Configuration updated"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    
    for (int i = 0; i < *count; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 8;
        
        tm_info.tm_sec += rand() % 60;
        mktime(&tm_info);
        
        strftime(entries[i].timestamp, 31, "%Y-%m-%d %H:%M:%S", &tm_info);
        strncpy(entries[i].level, levels[level_idx], 15);
        entries[i].level[15] = '\0';
        strncpy(entries[i].message, messages[msg_idx], 255);
        entries[i].message[255] = '\0';
        
        if (strcmp(levels[level_idx], "ERROR") == 0) entries[i].severity = 3;
        else if (strcmp(levels[level_idx], "WARNING") == 0) entries[i].severity = 2;
        else entries[i].severity = 1;
    }
}

int main() {
    srand((unsigned int)time(NULL));
    
    int entry_count;
    printf("Enter number of log entries to generate (1-%d): ", MAX_ENTRIES);
    if (scanf("%d", &entry_count) != 1 || entry_count <= 0 || entry_count > MAX_ENTRIES) {
        printf("Invalid input. Using default of 10 entries.\n");
        entry_count = 10;
    }
    
    struct LogEntry* log_entries = malloc(entry_count * sizeof(struct LogEntry));
    if (!log_entries) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    generate_sample_log