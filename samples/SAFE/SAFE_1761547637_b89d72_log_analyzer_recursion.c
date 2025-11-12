//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19 && strlen(level) >= 9) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void print_log_entry(const LogEntry* entry) {
    if (!entry) return;
    printf("%s %s %s\n", entry->timestamp, entry->level, entry->message);
}

void analyze_logs_recursive(LogEntry* entries, int count, int index, const char* target_level) {
    if (index >= count || !entries || !target_level) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        print_log_entry(&entries[index]);
    }
    
    analyze_logs_recursive(entries, count, index + 1, target_level);
}

int count_logs_by_level_recursive(LogEntry* entries, int count, int index, const char* target_level) {
    if (index >= count || !entries || !target_level) return 0;
    
    int current = (strcmp(entries[index].level, target_level) == 0) ? 1 : 0;
    return current + count_logs_by_level_recursive(entries, count, index + 1, target_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = '\0';
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    if (count == 0) {
        printf("No valid log entries found or file not found\n");
        return 1;
    }
    
    printf("Found %d log entries\n", count);
    
    char target_level[10];
    printf("Enter log level to filter (INFO, WARN, ERROR): ");
    if (!fgets(target_level, sizeof(target_level), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    target_level[strcspn(target_level, "\n")] = '\0';
    
    if (strlen(target_level) == 0) {
        printf("Invalid log level\n");
        return 1;
    }
    
    printf("\nLog entries with level '%s':\n", target_level);
    analyze_logs_recursive(entries, count, 0, target_level);
    
    int level_count = count_logs_by_level_recursive(entries, count, 0, target_level);
    printf("\nTotal %s entries: %d\n", target_level, level_count);
    
    return 0;
}