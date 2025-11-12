//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return entry->severity > 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!parse_log_line(line, &entry)) return 0;
    
    analyzer->entries[analyzer->count] = entry;
    analyzer->count++;
    
    switch (entry.severity) {
        case 3: analyzer->error_count++; break;
        case 2: analyzer->warning_count++; break;
        case 1: analyzer->info_count++; break;
    }
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:45 ERROR Database connection failed",
        "2024-01-15T10:33:20 INFO User login successful",
        "2024-01-15T10:34:10 WARNING Disk space running low",
        "2024-01-15T10:35:00 ERROR File not found",
        "2024-01-15T10:36:30 INFO Backup completed",
        "2024-01-15T10:37:45 WARNING Network latency high",
        "2024-01-15T10:38:20 INFO Cache cleared",
        "2024-01-15T10:39:05 ERROR Permission denied"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < sample_count && analyzer->count < MAX_ENTRIES; i++) {
        add_log_entry(analyzer, sample_logs[i]);
    }
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("INFO entries: %d\n", analyzer->info_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    
    if (analyzer->count > 0) {
        printf("Error percentage: %.1f%%\n", 
               (float)analyzer->error_count / analyzer->count * 100);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("\n%s entries:\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s - %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No %s entries found.\n", level);
    }
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf