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

int parse_log_recursive(FILE *file, LogEntry entries[], int index, int max_entries) {
    if (index >= max_entries) {
        return index;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return index;
    }
    
    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, index, max_entries);
    }
    
    LogEntry *entry = &entries[index];
    char *token = strtok(line, " ");
    int field = 0;
    
    while (token != NULL && field < 3) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token[token_len - 1] == '\n') {
            token[token_len - 1] = '\0';
            token_len--;
        }
        
        if (field == 0 && token_len < sizeof(entry->timestamp)) {
            strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        } else if (field == 1 && token_len < sizeof(entry->level)) {
            strncpy(entry->level, token, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
        } else if (field == 2 && token_len < sizeof(entry->message)) {
            strncpy(entry->message, token, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            
            char *next_token = strtok(NULL, "");
            if (next_token != NULL) {
                size_t remaining_len = strlen(next_token);
                if (remaining_len > 0 && remaining_len < sizeof(entry->message) - token_len - 1) {
                    strncat(entry->message, " ", sizeof(entry->message) - strlen(entry->message) - 1);
                    strncat(entry->message, next_token, sizeof(entry->message) - strlen(entry->message) - 1);
                }
            }
        }
        
        token = strtok(NULL, " ");
        field++;
    }
    
    return parse_log_recursive(file, entries, index + 1, max_entries);
}

void count_log_levels_recursive(const LogEntry entries[], int index, int total, int counts[]) {
    if (index >= total) {
        return;
    }
    
    const char *level = entries[index].level;
    if (strcmp(level, "INFO") == 0) {
        counts[0]++;
    } else if (strcmp(level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(level, "ERROR") == 0) {
        counts[2]++;
    } else if (strcmp(level, "DEBUG") == 0) {
        counts[3]++;
    } else {
        counts[4]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void display_logs_recursive(const LogEntry entries[], int index, int total) {
    if (index >= total) {
        return;
    }
    
    printf("Entry %d: [%s] %s - %s\n", 
           index + 1, 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);
    
    display_logs_recursive(entries, index + 1, total);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = parse_log_recursive(file, entries, 0, MAX_ENTRIES);
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 0;
    }
    
    printf("Found %d log entries:\n", entry_count);
    display_logs_recursive(entries, 0, entry_count);
    
    int level_counts[5] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    
    printf("\nLog Level Summary:\n");
    printf("INFO: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    printf("OTHER: %d\n", level_counts[4]);
    
    return 0;
}