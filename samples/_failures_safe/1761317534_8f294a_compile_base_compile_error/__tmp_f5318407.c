//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
};

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') {
            continue;
        }
        
        struct LogEntry* entry = &entries[count];
        char* parts[3];
        int part_count = 0;
        char* token = strtok(line, "|");
        
        while (token != NULL && part_count < 3) {
            parts[part_count] = token;
            part_count++;
            token = strtok(NULL, "|");
        }
        
        if (part_count == 3) {
            strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
            
            strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            
            strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Level Analysis:\n");
    printf("ERROR: %d entries\n", error_count);
    printf("WARNING: %d entries\n", warning_count);
    printf("INFO: %d entries\n", info_count);
    printf("TOTAL: %d entries\n", count);
}

void search_log_messages(struct LogEntry* entries, int count, const char* search_term) {
    printf("Search results for '%s':\n", search_term);
    int found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            printf("[%s] %s: %s", entries[i].timestamp, entries[i].level, entries[i].message);
            found_count++;
        }
    }
    
    printf("Found %d matching entries\n", found_count);
}

void display_recent_entries(struct LogEntry* entries, int count, int num_entries) {
    if (num_entries > count) {
        num_entries = count;
    }
    
    printf("Recent %d log entries:\n", num_entries);
    int start = count - num_entries;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char search_term[256];
    int choice;
    
    printf("Log File Analyzer\n");
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    printf("Successfully loaded %d log entries\n", entry_count);
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Analyze log levels\n");
        printf("2. Search log messages\n");
        printf("3. Display recent entries\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                analyze_log_levels(entries, entry_count);
                break;
            case 2:
                printf