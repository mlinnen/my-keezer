#ifndef JsonFileConfig_h
#define JsonFileConfig_h

#include <ArduinoJson.h>
#include "config.h"

class JsonFileConfig
{
  public:
    //JsonFileConfig();
    void loadConfiguration(const char *filename, Config &config);
    void saveConfiguration(const char *filename, const Config &config);
    void printFile(const char *filename);
};

#endif