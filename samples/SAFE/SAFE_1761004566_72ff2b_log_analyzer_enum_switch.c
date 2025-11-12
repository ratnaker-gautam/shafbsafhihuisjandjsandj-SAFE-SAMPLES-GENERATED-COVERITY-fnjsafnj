//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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
    char line[300];
    char level_str[20];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %19s %255[^\n]", 
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
    int occurrences = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            occurrences++;
        }
    }
    
    printf("Found %d %s entries\n", occurrences, level_to_string(target_level));
}

void print_log_summary(struct LogEntry* entries, int count) {
    int level_counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%s: %d\n", level_to_string(level), level_counts[level]);
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char filename[100];
    
    printf("Enter log filename: ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename\n");
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
    
    printf("Successfully read %d log entries\n", entry_count);
    
    int choice;
    do {
        printf("\nLog Analysis Menu:\n");
        printf("1. Show log summary\n");
        printf("2. Count DEBUG entries\n");
        printf("3. Count INFO entries\n");
        printf("4. Count WARNING entries\n");
        printf("5. Count ERROR entries\n");
        printf("6. Count CRITICAL entries\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                print_log_summary(entries, entry_count);
                break;
            case 2:
                analyze_log_level(entries, entry_count, LOG_DEBUG);
                break;
            case 3:
                analyze_log_level(entries, entry_count, LOG_INFO);
                break;
            case 4:
                analyze_log_level(entries, entry_count, LOG_WARNING);
                break;
            case 5:
                analyze_log_level(entries, entry_count, LOG_ERROR);
                break;
            case 6:
                analyze_log_level(entries, entry_count, LOG_CRITICAL);
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != 7);
    
    return 0;
}