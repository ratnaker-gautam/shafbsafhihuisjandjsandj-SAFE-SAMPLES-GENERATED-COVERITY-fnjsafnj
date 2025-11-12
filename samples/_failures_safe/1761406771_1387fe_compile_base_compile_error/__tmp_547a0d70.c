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
            case 1: stats->info_count++; level_counts[1]++; break;
            case 2: stats->warning_count++; level_counts[2]++; break;
            case 3: stats->error_count++; level_counts[3]++; break;
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

int read_log_file(const char* filename, struct LogEntry** entries_ptr) {
    if (!filename || !entries_ptr) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!entries) {
        fclose(file);
        return 0;
    }
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LEN, file) && count < MAX_ENTRIES) {
        line[MAX_LINE_LEN - 1] = '\0';
        if (strlen(line) > 1) {
            parse_log_line(line, &entries[count]);
            count++;
        }
    }
    
    fclose(file);
    *entries_ptr = entries;
    return count;
}

void generate_sample_log(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System started successfully",
        "User login detected",
        "Database connection established",
        "High memory usage detected",
        "Failed to connect to service",
        "Backup completed",
        "Security alert: multiple failed logins",
        "Configuration updated",
        "Disk space running low",
        "Network timeout occurred"
    };
    
    time_t now = time(NULL);
    
    for (int i = 0; i < 20; i++) {
        int level_idx = rand() % 3;
        int msg_idx = rand() % 10;
        
        time_t entry_time = now - (rand() % 86400);
        struct tm* entry_tm = localtime(&entry_time);
        
        fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d %s %s\n",
                entry_tm->tm_year + 1900, entry_tm->tm_mon + 1, entry_tm->tm_mday,
                entry_tm->tm_hour, entry_tm->tm_min, entry_tm->tm_sec,
                levels[level_idx], messages[msg_idx]);
    }
    
    fclose(file);
}

void