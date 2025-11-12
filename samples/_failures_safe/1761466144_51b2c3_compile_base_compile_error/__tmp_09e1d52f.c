//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LOG_ENTRIES 100
#define MAX_MESSAGE_LEN 256
#define MAX_TIMESTAMP_LEN 20

struct LogEntry {
    char timestamp[MAX_TIMESTAMP_LEN];
    char level[8];
    char message[MAX_MESSAGE_LEN];
    int severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_LOG_ENTRIES];
    int count;
    int error_count;
    int warning_count;
    int info_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int validate_timestamp(const char *timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
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

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->count >= MAX_LOG_ENTRIES) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    if (!validate_level(level)) return 0;
    if (strlen(message) >= MAX_MESSAGE_LEN) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    
    strncpy(entry->timestamp, timestamp, MAX_TIMESTAMP_LEN - 1);
    entry->timestamp[MAX_TIMESTAMP_LEN - 1] = '\0';
    
    strncpy(entry->level, level, 7);
    entry->level[7] = '\0';
    
    strncpy(entry->message, message, MAX_MESSAGE_LEN - 1);
    entry->message[MAX_MESSAGE_LEN - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) {
        entry->severity = 3;
        analyzer->error_count++;
    } else if (strcmp(level, "WARNING") == 0) {
        entry->severity = 2;
        analyzer->warning_count++;
    } else {
        entry->severity = 1;
        analyzer->info_count++;
    }
    
    analyzer->count++;
    return 1;
}

void display_statistics(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("ERROR entries: %d\n", analyzer->error_count);
    printf("WARNING entries: %d\n", analyzer->warning_count);
    printf("INFO entries: %d\n", analyzer->info_count);
    
    if (analyzer->count > 0) {
        printf("Error percentage: %.2f%%\n", (float)analyzer->error_count / analyzer->count * 100);
    }
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    if (!validate_level(level)) return;
    
    printf("Entries with level %s:\n", level);
    int found = 0;
    
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found with level %s\n", level);
    }
}

int main(void) {
    struct LogAnalyzer analyzer;
    init_log_analyzer(&analyzer);
    
    printf("Log Analyzer - Enter log entries (timestamp: YYYY-MM-DD HH:MM:SS, level: ERROR/WARNING/INFO, message)\n");
    printf("Type 'done' to finish input\n\n");
    
    char timestamp[MAX_TIMESTAMP_LEN];
    char level[8];
    char message[MAX_MESSAGE_LEN];
    
    while (analyzer.count < MAX_LOG_ENTRIES) {
        printf("Enter timestamp: ");