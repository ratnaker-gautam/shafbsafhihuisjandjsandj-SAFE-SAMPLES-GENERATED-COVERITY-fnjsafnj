//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int count, int* level_counts) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        level_counts[0]++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        level_counts[1]++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        level_counts[2]++;
    } else {
        level_counts[3]++;
    }
    
    count_log_levels_recursive(entries, index + 1, count, level_counts);
}

void display_logs_recursive(LogEntry* entries, int index, int count, const char* filter_level) {
    if (index >= count) return;
    
    if (filter_level == NULL || strcmp(entries[index].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_recursive(entries, index + 1, count, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
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
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file not accessible\n");
        return 1;
    }
    
    printf("\nFound %d log entries\n", entry_count);
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("INFO: %d\n", level_counts[0]);
    printf("WARNING: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    char choice[16];
    printf("\nDisplay logs? (all/info/warning/error/none): ");
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        return 1;
    }
    
    len = strlen(choice);
    if (len > 0 && choice[len - 1] == '\n') {
        choice[len - 1] = '\0';
    }
    
    printf("\n");
    if (strcmp(choice, "all") == 0) {
        display_logs_recursive(entries, 0, entry_count, NULL);
    } else if (strcmp(choice, "info") == 0) {
        display_logs_recursive(entries, 0, entry