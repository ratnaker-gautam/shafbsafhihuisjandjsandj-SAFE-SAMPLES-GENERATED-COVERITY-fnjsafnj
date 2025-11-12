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

void display_logs_recursive(LogEntry* entries, int start, int end, const char* level_filter) {
    if (start > end) return;
    
    if (level_filter == NULL || strcmp(entries[start].level, level_filter) == 0) {
        printf("%s | %s | %s\n", entries[start].timestamp, entries[start].level, entries[start].message);
    }
    
    display_logs_recursive(entries, start + 1, end, level_filter);
}

int read_log_file(const char* filename, LogEntry* entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && count < MAX_ENTRIES) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log filename: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries);
    if (entry_count == 0) {
        printf("No valid log entries found or file not accessible\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    analyze_log_recursive(entries, 0, entry_count - 1, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter level to filter (ERROR/WARNING/INFO/ALL): ");
    char filter[32];
    if (!fgets(filter, sizeof(filter), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    len = strlen(filter);
    if (len > 0 && filter[len - 1] == '\n') {
        filter[len - 1] = '\0';
    }
    
    const char* level_filter = NULL;
    if (strcmp(filter, "ERROR") == 0) {
        level_filter = "ERROR";
    } else if (strcmp(filter, "WARNING") == 0) {
        level_filter = "WARNING";
    } else if (strcmp(filter, "INFO") == 0) {
        level_filter = "INFO";
    }
    
    printf("\nFiltered Log Entries:\n");
    display_logs_recursive(entries, 0, entry_count - 1, level_filter);
    
    return 0;
}