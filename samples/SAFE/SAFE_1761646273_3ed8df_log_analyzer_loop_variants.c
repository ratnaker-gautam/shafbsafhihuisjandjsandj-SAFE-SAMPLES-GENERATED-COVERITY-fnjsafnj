//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int count = 0;
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    char line[MAX_LINE_LEN];
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        line[MAX_LINE_LEN - 1] = '\0';
        if (parse_log_line(line, &logs[count])) {
            count++;
        }
        i++;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int j = 0; j < count; j++) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[j].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    int k = count - 1;
    while (k >= 0 && found_errors < 5) {
        if (strcmp(logs[k].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[k].timestamp, logs[k].message);
            found_errors++;
        }
        k--;
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found\n");
    }
    
    printf("\nSearch for messages containing: ");
    char search_term[128];
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        return 0;
    }
    
    len = strlen(search_term);
    if (len > 0 && search_term[len - 1] == '\n') {
        search_term[len - 1] = '\0';
    }
    
    if (strlen(search_term) > 0) {
        printf("Matching entries:\n");
        int matches = 0;
        int m = 0;
        do {
            if (strstr(logs[m].message, search_term) != NULL) {
                printf("- [%s] %s: %s\n", logs[m].level, logs[m].timestamp, logs[m].message);
                matches++;
            }
            m++;
        } while (m < count);
        
        if (matches == 0) {
            printf("No matches found\n");
        }
    }
    
    return 0;
}