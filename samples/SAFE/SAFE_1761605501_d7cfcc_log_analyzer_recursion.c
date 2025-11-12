//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = strtok(temp, "|");
    parts[1] = strtok(NULL, "|");
    parts[2] = strtok(NULL, "\n");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
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

void display_entries_by_level(LogEntry* entries, int count, const char* level, int index) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, level) == 0) {
        printf("%s | %s | %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_entries_by_level(entries, count, level, index + 1);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, MAX_LINE_LENGTH, stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_log_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR entries:\n");
    display_entries_by_level(entries, entry_count, "ERROR", 0);
    
    printf("\nWARNING entries:\n");
    display_entries_by_level(entries, entry_count, "WARNING", 0);
    
    printf("\nINFO entries:\n");
    display_entries_by_level(entries, entry_count, "INFO", 0);
    
    return 0;
}