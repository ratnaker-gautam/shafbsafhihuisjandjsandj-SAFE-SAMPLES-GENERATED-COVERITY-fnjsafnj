//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int count_entries_by_level(struct LogEntry* entries, int count, int target_level, int index) {
    if (index >= count) return 0;
    int current = (entries[index].level == target_level) ? 1 : 0;
    return current + count_entries_by_level(entries, count, target_level, index + 1);
}

void print_level_summary(struct LogEntry* entries, int count, int level) {
    int total = count_entries_by_level(entries, count, level, 0);
    if (total > 0) {
        printf("%s: %d entries\n", level_to_string(level), total);
    }
}

void print_all_summaries(struct LogEntry* entries, int count, int current_level) {
    if (current_level > 4) return;
    print_level_summary(entries, count, current_level);
    print_all_summaries(entries, count, current_level + 1);
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    char level_str[32];
    char message[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%31s %1023[^\n]", level_str, message) != 2) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int read_all_entries(FILE* file, struct LogEntry* entries, int max_entries, int* count) {
    if (*count >= max_entries) return 0;
    if (read_log_entry(file, &entries[*count])) {
        (*count)++;
        return read_all_entries(file, entries, max_entries, count);
    }
    return 0;
}

int main(void) {
    printf("Log Analyzer - Enter log entries (format: LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    struct LogEntry entries[MAX_LEVELS];
    int entry_count = 0;
    
    char input[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_LEVELS) {
        printf("Log entry %d: ", entry_count + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) break;
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(input, "%31s %1023[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    print_all_summaries(entries, entry_count, 0);
    
    int total_entries = entry_count;
    printf("\nTotal entries: %d\n", total_entries);
    
    return 0;
}