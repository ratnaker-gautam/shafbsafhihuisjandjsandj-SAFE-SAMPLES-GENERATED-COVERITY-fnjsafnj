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
    parts[0] = strtok(temp, " ");
    parts[1] = strtok(NULL, " ");
    parts[2] = strtok(NULL, "\n");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (strlen(parts[0]) >= 20 || strlen(parts[1]) >= 10 || strlen(parts[2]) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

void analyze_log_recursive(LogEntry* entries, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_log_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void print_analysis_recursive(LogEntry* entries, int count, int index, const char* level_filter) {
    if (index >= count) return;
    
    if (level_filter == NULL || strcmp(entries[index].level, level_filter) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_analysis_recursive(entries, count, index + 1, level_filter);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
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
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter level to filter (ERROR/WARNING/INFO/ALL): ");
    char filter[20];
    if (fgets(filter, 20, stdin) != NULL) {
        filter[strcspn(filter, "\n")] = '\0';
        
        printf("\nFiltered Log Entries:\n");
        if (strcmp(filter, "ALL") == 0) {
            print_analysis_recursive(entries, entry_count, 0, NULL);
        } else if (strcmp(filter, "ERROR") == 0 || strcmp(filter, "WARNING") == 0 || strcmp(filter, "INFO") == 0) {
            print_analysis_recursive(entries, entry_count, 0, filter);
        } else {
            printf("Invalid filter. Showing all entries:\n");
            print_analysis_recursive(entries, entry_count, 0, NULL);
        }
    }
    
    return 0;
}