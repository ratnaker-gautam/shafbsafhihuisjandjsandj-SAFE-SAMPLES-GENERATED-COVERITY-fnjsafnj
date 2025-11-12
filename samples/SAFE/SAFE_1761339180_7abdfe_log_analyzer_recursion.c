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

void print_log_summary(LogEntry* entries, int count, int error_count, int warning_count, int info_count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (count > 0) {
        printf("\nFirst 5 entries:\n");
        int display_count = count < 5 ? count : 5;
        for (int i = 0; i < display_count; i++) {
            printf("%d. [%s] %s: %s\n", i + 1, entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message). Enter 'END' to finish:\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS|LEVEL|message\n");
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
    
    print_log_summary(entries, entry_count, error_count, warning_count, info_count);
    
    return 0;
}