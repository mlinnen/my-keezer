#ifndef JsonFile_h
#define JsonFile_h

#include <FS.h>
#include <ArduinoJson.h>
#include "config.h"

void jsonfile_load(const char *filename, Config &config);
void jsonfile_save(const char *filename, const Config &config);
void jsonfile_print(const char *filename);

#endif