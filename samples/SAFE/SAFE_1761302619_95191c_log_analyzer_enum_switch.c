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
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("Found at position %d: [%s] %s - %s\n", 
                   i, entries[i].timestamp, 
                   level_to_string(entries[i].level), 
                   entries[i].message);
            found++;
        }
    }
    
    printf("Total %s entries: %d\n", level_to_string(target_level), found);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char filename[256];
    
    printf("Enter log filename: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename input\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (entry_count < 100) {
        int result = read_log_entry(file, &entries[entry_count]);
        if (result == 0) {
            break;
        } else if (result == 1) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    printf("Loaded %d log entries\n", entry_count);
    
    int choice;
    printf("Select log level to analyze:\n");
    printf("0: DEBUG\n1: INFO\n2: WARNING\n3: ERROR\n4: CRITICAL\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 4) {
        printf("Invalid choice range\n");
        return 1;
    }
    
    enum LogLevel target_level = choice;
    analyze_log_level(entries, entry_count, target_level);
    
    return 0;
}