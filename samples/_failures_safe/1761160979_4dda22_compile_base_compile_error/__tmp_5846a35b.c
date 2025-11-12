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
    int found = 0;
    
    printf("Entries with level %s:\n", level_to_string(target_level));
    printf("----------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No entries found for level %s\n", level_to_string(target_level));
    }
    printf("\n");
}

void count_log_levels(struct LogEntry* entries, int count) {
    int counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            counts[entries[i].level]++;
        }
    }
    
    printf("Log Level Statistics:\n");
    printf("----------------------------------------\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
    printf("\n");
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char filename[256];
    FILE* file;
    
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
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Successfully loaded %d log entries.\n\n", entry_count);
    
    int choice;
    do {
        printf("Log Analyzer Menu:\n");
        printf("1. Show all entries by level\n");
        printf("2. Show statistics\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Select log level:\n");
                printf("0. DEBUG\n");
                printf("1. INFO\n");
                printf("2. WARNING\n");
                printf("3. ERROR\n");
                printf("4. CRITICAL\n");
                printf("Enter level: ");
                
                int level_choice;
                if (scanf("%d", &level_choice) != 1 || 
                    level_choice < 0 || level_choice > 4) {
                    printf("Invalid level selection.\n\n");
                    while (getchar() != '\n');
                    break;
                }
                
                analyze_log_level(entries