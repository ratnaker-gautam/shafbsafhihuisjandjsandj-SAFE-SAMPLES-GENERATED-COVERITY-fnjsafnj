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

void display_logs_recursive(LogEntry* entries, int count, int index, const char* filter_level) {
    if (index >= count) return;
    
    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s | %s | %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, count, index + 1, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be opened\n");
        return 1;
    }
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter log level to filter (ERROR/WARNING/INFO/ALL): ");
    char filter[20];
    
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filter[strcspn(filter, "\n")] = '\0';
    
    printf("\nFiltered Log Entries:\n");
    if (strcmp(filter, "ALL") == 0) {
        display_logs_recursive(entries, entry_count, 0, NULL);
    } else if (strcmp(filter, "ERROR") == 0 || strcmp(filter, "WARNING") == 0 || strcmp(filter, "INFO") == 0) {
        display