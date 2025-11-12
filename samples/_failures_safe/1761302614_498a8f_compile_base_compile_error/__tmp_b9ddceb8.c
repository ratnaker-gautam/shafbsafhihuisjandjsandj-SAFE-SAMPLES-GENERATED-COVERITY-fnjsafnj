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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* tokens[3];
    char* token = strtok(temp_line, "|");
    int token_count = 0;
    
    while (token != NULL && token_count < 3) {
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        tokens[token_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (token_count != 3) return 0;
    
    strncpy(entry->timestamp, tokens[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, tokens[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, tokens[2], 255);
    entry->message[255] = '\0';
    
    entry->error_count = 0;
    if (strstr(entry->level, "ERROR") != NULL) {
        entry->error_count = 1;
    }
    
    return 1;
}

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS|LEVEL|MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entry limit reached.\n");
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_timestamp(entry.timestamp)) {
                if (entry_count < MAX_ENTRIES) {
                    entries[entry_count] = entry;
                    total_errors += entry.error_count;
                    entry_count++;
                } else {
                    printf("Entry buffer full.\n");
                    break;
                }
            } else {
                printf("Invalid timestamp format: %s\n", entry.timestamp);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    int info_count = 0;
    int warning_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strstr(entries[i].level, "INFO") != NULL) {
            info_count++;
        } else if (strstr(entries[i].level, "WARNING") != NULL) {
            warning_count++;
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Info entries: %d\n", info_count);
    printf("Warning entries: %d\n", warning_count);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / entry_count);
    
    printf("\nFirst 10 entries (sorted by timestamp):\n");
    int display_count = (entry_count <