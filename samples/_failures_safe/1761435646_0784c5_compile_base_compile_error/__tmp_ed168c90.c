//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs_recursive(LogEntry* logs, int count, int index, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(logs[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(logs[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(logs[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    analyze_logs_recursive(logs, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(LogEntry* logs, int count, int index, const char* filter_level) {
    if (index >= count) return;
    
    if (filter_level == NULL || strcmp(logs[index].level, filter_level) == 0) {
        printf("%s %s %s\n", logs[index].timestamp, logs[index].level, logs[index].message);
    }
    
    display_logs_recursive(logs, count, index + 1, filter_level);
}

int read_log_file(const char* filename, LogEntry* logs, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (parse_log_line(line, &logs[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
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
    
    int log_count = read_log_file(filename, logs, MAX_ENTRIES);
    if (log_count == 0) {
        printf("No logs found or error reading file\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_logs_recursive(logs, log_count, 0, &error_count, &warning_count, &info_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nEnter log level to filter (or 'all' for all): ");
    char filter[32];
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    len = strlen(filter);
    if (len > 0 && filter[len - 1] == '\n') {
        filter[len - 1] = '\0';
    }
    
    const char* filter_ptr = NULL;
    if (strcmp(filter, "all") != 0) {
        if (strcmp(filter, "ERROR") == 0 || strcmp(filter, "WARNING") == 0 || strcmp(filter, "INFO") == 0) {
            filter_ptr = filter;
        } else {
            printf("Invalid filter. Showing all