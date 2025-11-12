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
    analyzer->count = 0;
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->info_count = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_severity(level);

    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;

    analyzer->count++;
    return 1;
}

void generate_sample_data(struct LogAnalyzer *analyzer) {
    add_entry(analyzer, "2024-01-15 10:30:25", "INFO", "System started successfully");
    add_entry(analyzer, "2024-01-15 10:35:42", "WARNING", "High memory usage detected");
    add_entry(analyzer, "2024-01-15 10:40:18", "ERROR", "Database connection failed");
    add_entry(analyzer, "2024-01-15 10:45:33", "INFO", "Backup completed");
    add_entry(analyzer, "2024-01-15 10:50:07", "ERROR", "File system corruption detected");
    add_entry(analyzer, "2024-01-15 10:55:21", "WARNING", "CPU temperature above threshold");
    add_entry(analyzer, "2024-01-15 11:00:14", "INFO", "User login successful");
}

void display_summary(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", analyzer->count);
    printf("Errors: %d\n", analyzer->error_count);
    printf("Warnings: %d\n", analyzer->warning_count);
    printf("Info messages: %d\n", analyzer->info_count);
    printf("\n");
}

void display_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("Entries with level '%s':\n", level);
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
                   analyzer->entries[i].level, analyzer->entries[i].message);
        }
    }
    printf("\n");
}

void display_all_entries(const struct LogAnalyzer *analyzer) {
    printf("All log entries:\n");
    for (int i = 0; i < analyzer->count; i++) {
        printf("[%s] %s: %s\n", analyzer->entries[i].timestamp, 
               analyzer->entries[i].level, analyzer->entries[i].message);
    }
    printf("\n");
}

int main() {
    struct LogAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    generate_sample_data(&analyzer);
    
    int choice;
    do {
        printf("Log Analyzer Menu:\n");
        printf("1. Show summary\n");
        printf("2. Show all entries\n");
        printf("3. Show ERROR entries\n");
        printf("4. Show WARNING entries\n");
        printf("5. Show INFO entries\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                display_summary(&analyzer);
                break;
            case 2:
                display_all_entries(&analyzer);
                break;
            case 3:
                display_entries_by_level(&