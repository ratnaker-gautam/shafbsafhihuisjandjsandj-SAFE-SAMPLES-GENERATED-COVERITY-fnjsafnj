//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = strtok(temp, " ");
    parts[1] = strtok(NULL, " ");
    parts[2] = strtok(NULL, "\n");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (strlen(parts[0]) >= 20 || strlen(parts[1]) >= 10 || strlen(parts[2]) >= MAX_LINE_LEN) return 0;
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void print_entries_by_level_recursive(LogEntry* entries, int count, int index, const char* target_level) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_entries_by_level_recursive(entries, count, index + 1, target_level);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, MAX_LINE_LEN, stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Summary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter level to filter (ERROR/WARNING/INFO): ");
    char filter_level[10];
    if (fgets(filter_level, 10, stdin)) {
        filter_level[strcspn(filter_level, "\n")] = '\0';
        
        if (strcmp(filter_level, "ERROR") == 0 || strcmp(filter_level, "WARNING") == 0 || strcmp(filter_level, "INFO") == 0) {
            printf("\nEntries with level %s:\n", filter_level);
            print_entries_by_level_recursive(entries, entry_count, 0, filter_level);
        } else {
            printf("Invalid level specified.\n");
        }
    }
    
    return 0;
}