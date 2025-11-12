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
    
    entry->level = (enum LogLevel)level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int level_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            level_count++;
        }
    }
    
    printf("Found %d %s entries\n", level_count, level_to_string(target_level));
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    while (entry_count < 1000) {
        int result = read_log_entry(file, &entries[entry_count]);
        if (result == 0) {
            break;
        }
        if (result == 1) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return;
    }
    
    printf("Log analysis for %s (%d entries):\n", filename, entry_count);
    
    switch (entries[0].level) {
        case LOG_DEBUG:
            printf("First entry is DEBUG level\n");
            break;
        case LOG_INFO:
            printf("First entry is INFO level\n");
            break;
        case LOG_WARNING:
            printf("First entry is WARNING level\n");
            break;
        case LOG_ERROR:
            printf("First entry is ERROR level\n");
            break;
        case LOG_CRITICAL:
            printf("First entry is CRITICAL level\n");
            break;
        default:
            printf("First entry has unknown level\n");
            break;
    }
    
    analyze_log_level(entries, entry_count, LOG_DEBUG);
    analyze_log_level(entries, entry_count, LOG_INFO);
    analyze_log_level(entries, entry_count, LOG_WARNING);
    analyze_log_level(entries, entry_count, LOG_ERROR);
    analyze_log_level(entries, entry_count, LOG_CRITICAL);
    
    int has_errors = 0;
    for (int i = 0; i < entry_count; i++) {
        switch (entries[i].level) {
            case LOG_ERROR:
            case LOG_CRITICAL:
                if (!has_errors) {
                    printf("\nError/Critical entries found:\n");
                    has_errors = 1;
                }
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
                break;
            default:
                break;
        }
    }
    
    if (!has_errors) {
        printf("\nNo error or critical entries found\n");
    }
}

int main() {
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }
    
    process_log_file(filename);
    
    return 0;
}