//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= (MAX_LINE_LENGTH - 30)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30 - 1);
    entry->message[MAX_LINE_LENGTH - 30 - 1] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0, other_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else other_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void search_log_messages(struct LogEntry* entries, int count, const char* keyword) {
    if (entries == NULL || count <= 0 || keyword == NULL) return;
    
    printf("Messages containing '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No messages found containing '%s'\n", keyword);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (filename == NULL || entries == NULL || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
    }
    
    fclose(file);
    return entry_count;
}

void display_recent_entries(struct LogEntry* entries, int count, int num_entries) {
    if (entries == NULL || count <= 0 || num_entries <= 0) return;
    
    if (num_entries > count) num_entries = count;
    
    printf("Recent %d log entries:\n", num_entries);
    int start = count - num_entries;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char search_term[100];
    int choice;
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be opened\n");
        return 1;
    }
    
    printf("Successfully loaded %d log entries\n", entry_count);
    
    while (1) {
        printf("\nLog Analyzer Menu:\n");
        printf("1. Show log level analysis\n");
        printf("2. Search messages\n");