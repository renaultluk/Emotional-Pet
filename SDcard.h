#ifndef _SDCARD_H
#define _SDCARD_H

// some utility
// determine have userSetting.txt or not
bool haveSetting(fs::FS &fs);

String readFileline(fs::FS &fs, const char * path, int line);
void appendFile(fs::FS &fs, const char * path, const char * message);
void writeFile(fs::FS &fs, const char * path, const char * message);
void createDir(fs::FS &fs, const char * path);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void deleteFile(fs::FS &fs, const char * path);
//------------------------------------------------------------------------//
void cardType(uint8_t cardType);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void testFileIO(fs::FS &fs, const char * path);

#endif
