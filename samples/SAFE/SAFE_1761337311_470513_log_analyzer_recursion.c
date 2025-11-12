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
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9) return 0;
    
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

void analyze_logs_recursive(LogEntry* logs, int count, int index, const char* target_level) {
    if (index >= count || !logs || !target_level) return;
    
    if (strcmp(logs[index].level, target_level) == 0) {
        print_log_entry(&logs[index]);
    }
    
    analyze_logs_recursive(logs, count, index + 1, target_level);
}

int read_log_file(const char* filename, LogEntry* logs, int max_entries) {
    if (!filename || !logs || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = '\0';
            if (parse_log_line(line, &logs[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    char filename[256];
    char target_level[10];
    LogEntry logs[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    printf("Enter log level to filter (INFO, WARN, ERROR): ");
    if (!fgets(target_level, sizeof(target_level), stdin)) {
        printf("Error reading log level\n");
        return 1;
    }
    target_level[strcspn(target_level, "\n")] = '\0';
    
    if (strlen(target_level) == 0) {
        printf("Invalid log level\n");
        return 1;
    }
    
    int log_count = read_log_file(filename, logs, MAX_ENTRIES);
    if (log_count == 0) {
        printf("No valid log entries found or file not accessible\n");
        return 1;
    }
    
    printf("\nFiltered log entries for level '%s':\n", target_level);
    analyze_logs_recursive(logs, log_count, 0, target_level);
    
    return 0;
}