//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void find_most_frequent_message(const LogEntry* entries, int count, char* result) {
    int max_count = 0;
    result[0] = '\0';
    
    for (int i = 0; i < count; i++) {
        int current_count = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(entries[i].message, entries[j].message) == 0) {
                current_count++;
            }
        }
        if (current_count > max_count) {
            max_count = current_count;
            strcpy(result, entries[i].message);
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warn_count = count_log_level(entries, entry_count, "WARN");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    
    char most_frequent[256];
    find_most_frequent_message(entries, entry_count, most_frequent);
    printf("Most frequent message: %s\n", most_frequent);
    
    int unique_messages = 0;
    for (int i = 0; i < entry_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(entries[i].message, entries[j].message) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) unique_messages++;
    }
    printf("Unique messages: %d\n", unique_messages);
    
    return 0;
}