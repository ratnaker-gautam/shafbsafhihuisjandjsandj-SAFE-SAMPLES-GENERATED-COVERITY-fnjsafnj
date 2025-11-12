//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
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

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    
    int year, month, day, hour, min, sec;
    if (sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != 6) {
        return 0;
    }
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    
    snprintf(timestamp, 20, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    return 1;
}

int parse_log_level(const char *str, char *level, int *severity) {
    if (str == NULL || level == NULL || severity == NULL) return 0;
    
    if (strcasecmp(str, "ERROR") == 0) {
        strncpy(level, "ERROR", 10);
        *severity = 3;
        return 1;
    } else if (strcasecmp(str, "WARNING") == 0) {
        strncpy(level, "WARNING", 10);
        *severity = 2;
        return 1;
    } else if (strcasecmp(str, "INFO") == 0) {
        strncpy(level, "INFO", 10);
        *severity = 1;
        return 1;
    }
    
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, 
                  const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (analyzer->count >= MAX_ENTRIES) {
        return 0;
    }
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) {
        return 0;
    }
    
    if (!parse_log_level(level, entry->level, &entry->severity)) {
        return 0;
    }
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    analyzer->count++;
    
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;
    
    return 1;
}

void generate_sample_logs(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    add_log_entry(analyzer, "2024-03-15 10:30:45", "INFO", "System startup completed");
    add_log_entry(analyzer, "2024-03-15 10:35:22", "WARNING", "High memory usage detected");
    add_log_entry(analyzer, "2024-03-15 11:20:15", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-03-15 11:45:30", "INFO", "Backup process started");
    add_log_entry(analyzer, "2024-03-15 12:15:08", "WARNING", "CPU temperature above threshold");
    add_log_entry(analyzer, "2024-03-15 13:05:42", "ERROR", "Network timeout occurred");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    printf("Entries with level '%s':\n", level);
    int found = 0;
    for (int i = 0; i < analyzer->count; i++) {
        if (strcasecmp(analyzer->entries[i].level, level) == 0) {
            printf("%s [%s] %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found for