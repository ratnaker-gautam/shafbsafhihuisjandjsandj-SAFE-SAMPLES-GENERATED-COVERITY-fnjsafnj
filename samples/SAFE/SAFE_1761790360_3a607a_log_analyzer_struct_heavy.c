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

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) - 1) return 0;
    if (strlen(level) >= sizeof(entry->level) - 1) return 0;
    if (strlen(message) >= sizeof(entry->message) - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
    } else if (strcmp(level, "INFO") == 0) {
        entry->severity = 1;
    } else {
        entry->severity = 0;
    }
    
    return 1;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer == NULL || line == NULL) return 0;
    if (analyzer->count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    if (parse_log_line(line, entry)) {
        analyzer->count++;
        if (entry->severity == 3) analyzer->error_count++;
        else if (entry->severity == 2) analyzer->warning_count++;
        else if (entry->severity == 1) analyzer->info_count++;
        return 1;
    }
    return 0;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 INFO User login: john_doe",
        "2024-01-15T10:33:45 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO Backup completed",
        "2024-01-15T10:35:20 WARNING Disk space low",
        "2024-01-15T10:36:05 ERROR Service unavailable",
        "2024-01-15T10:37:00 INFO Cache cleared",
        NULL
    };
    
    for (int i = 0; sample_logs[i] != NULL; i++) {
        if (analyzer->count < MAX_ENTRIES) {
            add_log_entry(analyzer, sample_logs[i]);
        }
    }
}

void display_summary(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("Entries with level '%s':\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
    printf("\n");
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Log Analyzer - Processing sample logs...\n\n");
    generate_sample_logs(&analyzer);
    
    display_summary(&analyzer);
    
    display_entries_by_level(&analyzer, "ERROR");
    display_entries_by_level(&analyzer, "WARNING");
    
    return 0;
}