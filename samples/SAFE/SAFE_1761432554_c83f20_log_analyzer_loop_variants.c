//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token != NULL && count < 3) {
        parts[count] = token;
        count++;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    if (strlen(parts[0]) < sizeof(entry->timestamp) &&
        strlen(parts[1]) < sizeof(entry->level) &&
        strlen(parts[2]) < sizeof(entry->message)) {
        strcpy(entry->timestamp, parts[0]);
        strcpy(entry->level, parts[1]);
        strcpy(entry->message, parts[2]);
        return 1;
    }
    
    return 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    int i = 0;
    
    while (i < count) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    int j;
    for (j = 0; j < count; j++) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            printf("Error found: %s - %s\n", entries[j].timestamp, entries[j].message);
        }
    }
    
    int k = 0;
    do {
        if (k < count && strcmp(entries[k].level, "WARNING") == 0) {
            printf("Warning: %s\n", entries[k].message);
        }
        k++;
    } while (k < count);
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char input[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message). Enter 'END' to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "END") == 0) break;
        
        if (strlen(input) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(input, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}