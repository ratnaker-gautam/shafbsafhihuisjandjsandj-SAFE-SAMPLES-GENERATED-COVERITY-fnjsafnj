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

void analyze_log_level(FILE* file, enum LogLevel target_level) {
    struct LogEntry entry;
    int count = 0;
    
    printf("Entries with level %s:\n", level_to_string(target_level));
    printf("========================\n");
    
    rewind(file);
    
    while (read_log_entry(file, &entry) > 0) {
        if (entry.level == target_level) {
            printf("%s %s %s\n", entry.timestamp, 
                   level_to_string(entry.level), entry.message);
            count++;
        }
    }
    
    printf("\nTotal: %d entries\n", count);
}

void show_statistics(FILE* file) {
    struct LogEntry entry;
    int counts[5] = {0};
    
    rewind(file);
    
    while (read_log_entry(file, &entry) > 0) {
        if (entry.level >= LOG_DEBUG && entry.level <= LOG_CRITICAL) {
            counts[entry.level]++;
        }
    }
    
    printf("Log Level Statistics:\n");
    printf("====================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
}

int main(void) {
    char filename[256];
    FILE* file;
    int choice;
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    while (1) {
        printf("\nLog Analyzer Menu:\n");
        printf("1. Show DEBUG entries\n");
        printf("2. Show INFO entries\n");
        printf("3. Show WARNING entries\n");
        printf("4. Show ERROR entries\n");
        printf("5. Show CRITICAL entries\n");
        printf("6. Show statistics\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            fclose(file);
            return 1;
        }
        
        switch (choice) {
            case 1:
                analyze_log_level(file, LOG_DEBUG);
                break;
            case 2:
                analyze_log_level(file, LOG_INFO);
                break;
            case 3:
                analyze_log_level(file, LOG_WARNING);
                break;
            case 4:
                analyze_log_level(file, LOG_ERROR);
                break;
            case 5:
                analyze_log_level(file, LOG_CRITICAL);
                break;
            case 6:
                show_statistics(file);
                break;
            case 7:
                fclose(file);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    
    fclose(file);
    return 0;
}