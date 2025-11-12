//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;

    while (fgets(line, sizeof(line), file) && line_count < MAX_ENTRIES) {
        if (strlen(line) == 0 || line[0] == '\n') continue;

        line[strcspn(line, "\n")] = 0;

        if (strlen(line) < 50) continue;

        int scanned = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (scanned == 3) {
            for (int i = 0; i < strlen(entry.level); i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
            line_count++;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <logfile> <mode>\n", argv[0]);
        fprintf(stderr, "Modes: count, errors, warnings\n");
        return 1;
    }

    if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0) {
        fprintf(stderr, "Error: Empty filename or mode\n");
        return 1;
    }

    LogProcessor processor = NULL;

    if (strcmp(argv[2], "count") == 0) {
        processor = count_entries;
    } else if (strcmp(argv[2], "errors") == 0) {
        processor = show_errors;
    } else if (strcmp(argv[2], "warnings") == 0) {
        processor = show_warnings;
    } else {
        fprintf(stderr, "Error: Invalid mode. Use: count, errors, warnings\n");
        return 1;
    }

    process_log_file(argv[1], processor);
    return 0;
}