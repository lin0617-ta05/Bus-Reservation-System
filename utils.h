#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void clearScreen(void);
void pauseScreen(void);
void printBanner(const char *subTitle);
void printCollegeHeader(void);

void trimString(char *str);
int getInt(const char *prompt, int minVal, int maxVal);
void getString(const char *prompt, char *buffer, int maxLen, int allowEmpty);
void getDate(const char *prompt, char *buffer, int maxLen);
int confirmAction(const char *prompt);

#endif /* UTILS_H */
