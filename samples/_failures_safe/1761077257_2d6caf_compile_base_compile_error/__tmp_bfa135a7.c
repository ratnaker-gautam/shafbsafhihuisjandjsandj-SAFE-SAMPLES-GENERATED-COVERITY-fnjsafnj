//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer->count >= MAX_ENTRIES) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;

    struct LogEntry *entry = &analyzer->entries[analyzer->count];
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = parse_log_level(level);

    analyzer->count++;
    if (entry->severity == 3) analyzer->error_count++;
    else if (entry->severity == 2) analyzer->warning_count++;
    else if (entry->severity == 1) analyzer->info_count++;

    return 1;
}

void generate_sample_data(struct LogAnalyzer *analyzer) {
    add_log_entry(analyzer, "2024-01-15 10:30:45", "INFO", "System started successfully");
    add_log_entry(analyzer, "2024-01-15 10:31:22", "WARN", "High memory usage detected");
    add_log_entry(analyzer, "2024-01-15 10:32:10", "ERROR", "Database connection failed");
    add_log_entry(analyzer, "2024-01-15 10:33:05", "INFO", "User login successful");
    add_log_entry(analyzer, "2024-01-15 10:34:18", "ERROR", "File not found: config.xml");
    add_log_entry(analyzer, "2024-01-15 10:35:42", "WARN", "Slow response time detected");
    add_log_entry(analyzer, "2024-01-15 10:36:27", "INFO", "Backup completed");
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
    int target_severity = parse_log_level(level);
    printf("Entries with level '%s':\n", level);
    
    for (int i = 0; i < analyzer->count; i++) {
        if (analyzer->entries[i].severity == target_severity) {
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

int get_user_choice(void) {
    char input[16];
    printf("Choose an option:\n");
    printf("1. Show summary\n");
    printf("2. Show all entries\n");
    printf("3. Show errors\n");
    printf("4. Show warnings\n");
    printf("5. Show info messages\n");
    printf("6. Exit\n");
    printf("Enter choice (1-6): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1) return -1;
    if (!isdigit(input[0])) return -1;
    
    int choice = input[0] - '0';
    if (choice < 1 || choice > 6) return -1;
    
    return choice