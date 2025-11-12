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
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 20 || strlen(parts[1]) >= 10 || strlen(parts[2]) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(LogEntry* entries, int start, int end, int* error_count, int* warning_count, int* info_count) {
    if (start > end) return;
    
    if (strcmp(entries[start].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[start].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[start].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_log_recursive(entries, start + 1, end, error_count, warning_count, info_count);
}

void display_log_summary(int error_count, int warning_count, int info_count, int total_count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", total_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (total_count > 0) {
        printf("Error percentage: %.1f%%\n", (error_count * 100.0) / total_count);
        printf("Warning percentage: %.1f%%\n", (warning_count * 100.0) / total_count);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(entries, 0, entry_count - 1, &error_count, &warning_count, &info_count);
    
    display_log_summary(error_count, warning_count, info_count, entry_count);
    
    return 0;
}