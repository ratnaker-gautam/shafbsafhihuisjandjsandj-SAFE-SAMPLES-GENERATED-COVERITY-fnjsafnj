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
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19) {
        strncpy(entry->timestamp, timestamp, 19);
        entry->timestamp[19] = '\0';
    } else {
        return 0;
    }
    
    if (strlen(level) >= 9) {
        strncpy(entry->level, level, 9);
        entry->level[9] = '\0';
    } else {
        return 0;
    }
    
    if (strlen(message) >= MAX_LINE_LENGTH - 1) {
        strncpy(entry->message, message, MAX_LINE_LENGTH - 2);
        entry->message[MAX_LINE_LENGTH - 2] = '\0';
    } else {
        strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
        entry->message[MAX_LINE_LENGTH - 1] = '\0';
    }
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int count, int* level_counts) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        level_counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        level_counts[1]++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        level_counts[2]++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        level_counts[3]++;
    } else {
        level_counts[4]++;
    }
    
    count_log_levels_recursive(entries, index + 1, count, level_counts);
}

void print_analysis_recursive(LogEntry* entries, int index, int count, const char* search_term) {
    if (index >= count) return;
    
    if (search_term && search_term[0] != '\0') {
        if (strstr(entries[index].message, search_term) != NULL) {
            printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
        }
    } else {
        printf("%s %s %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }
    
    print_analysis_recursive(entries, index + 1, count, search_term);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char filename[256];
    char search_term[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (filename[0] == '\0') {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("Enter search term (or press Enter for all entries): ");
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        fprintf(stderr, "Error reading search term\n");
        return 1;
    }
    
    len = strlen(search_term);
    if (len > 0 && search_term[len - 1] == '\n') {
        search_term[len - 1] = '\0';
    }
    
    int level_counts[5] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN:  %d\n",