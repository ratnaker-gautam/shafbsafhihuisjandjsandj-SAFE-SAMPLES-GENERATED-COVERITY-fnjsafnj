//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    
    entry->level = level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int matches = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            matches++;
        }
    }
    
    printf("Found %d %s entries\n", matches, level_to_string(target_level));
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    while (entry_count < 1000) {
        int result = read_log_entry(file, &entries[entry_count]);
        if (result == 0) break;
        if (result == 1) entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return;
    }
    
    printf("Log analysis for %s:\n", filename);
    printf("Total entries: %d\n", entry_count);
    
    analyze_log_level(entries, entry_count, LOG_DEBUG);
    analyze_log_level(entries, entry_count, LOG_INFO);
    analyze_log_level(entries, entry_count, LOG_WARNING);
    analyze_log_level(entries, entry_count, LOG_ERROR);
    analyze_log_level(entries, entry_count, LOG_CRITICAL);
    
    int choice;
    printf("\nSelect log level to view (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return;
    }
    
    if (choice < 0 || choice > 4) {
        printf("Invalid log level selection\n");
        return;
    }
    
    enum LogLevel selected_level = choice;
    printf("\nEntries with level %s:\n", level_to_string(selected_level));
    
    int found = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == selected_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            found++;
        }
    }
    
    if (!found) {
        printf("No entries found for selected level\n");
    }
}

int main() {
    char filename[256];
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (strlen(filename) == 0) {
        printf("Filename cannot be empty\n");
        return 1;
    }
    
    process_log_file(filename);
    return 0;
}