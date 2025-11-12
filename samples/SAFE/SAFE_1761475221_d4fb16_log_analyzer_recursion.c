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
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19 && strlen(level) >= 2 && strlen(message) >= 1) {
        strncpy(entry->timestamp, timestamp, 19);
        entry->timestamp[19] = '\0';
        strncpy(entry->level, level, 9);
        entry->level[9] = '\0';
        strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
        entry->message[MAX_LINE_LENGTH - 1] = '\0';
        return 1;
    }
    
    return 0;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        counts[2]++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        counts[3]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void display_logs_by_level_recursive(LogEntry* entries, int index, int total, const char* target_level) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, target_level) == 0) {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    display_logs_by_level_recursive(entries, index + 1, total, target_level);
}

int read_log_file(const char* filename, LogEntry* entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file is empty.\n");
        return 1;
    }
    
    printf("Loaded %d log entries.\n", entry_count);
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN:  %d\n", level_counts[1]);
    printf("INFO:  %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    
    printf("\nEnter log level to display (ERROR/WARN/INFO/DEBUG): ");
    char level_choice[10];
    if (fgets(level_choice, 10, stdin)) {
        level_choice[strcspn(level_choice, "\n")] = '\0';
        
        if (strcmp(level_choice, "ERROR") == 0 || strcmp(level_choice, "WARN") == 0 ||
            strcmp(level_choice, "INFO") == 0 || strcmp(level_choice, "DEBUG") == 0) {
            printf("\nLogs with level %s:\n", level_choice);
            display_logs_by_level_recursive(entries, 0, entry_count, level_choice);
        } else {
            printf("Invalid log level specified.\n");
        }
    }
    
    return 0;
}