//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    char* message = strtok(NULL, "");
    
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

void count_log_levels_recursive(LogEntry* entries, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    if (!entries || !error_count || !warning_count || !info_count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        if (*error_count < INT_MAX) (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        if (*warning_count < INT_MAX) (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        if (*info_count < INT_MAX) (*info_count)++;
    }
    
    count_log_levels_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(LogEntry* entries, int count, int index, const char* filter_level) {
    if (index >= count) return;
    if (!entries) return;
    
    if (!filter_level || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, count, index + 1, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return -1;
    
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
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

int main(void) {
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
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (count <= 0) {
        printf("No valid log entries found or file error\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    count_log_levels_recursive(entries, count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    printf("\nDisplay logs (all/error/warning/info): ");
    char choice[20];
    
    if (!fgets(choice, sizeof(choice), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    choice[strcspn(choice, "\n")] = '\0';
    
    const char* filter = NULL;
    if (strcmp(choice, "error") == 0) filter = "ERROR";
    else if (strcmp(choice, "warning") == 0) filter = "WARNING";
    else if (strcmp(choice, "info") == 0) filter = "INFO";
    
    printf("\nLog entries:\