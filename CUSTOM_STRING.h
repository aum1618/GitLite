#pragma once
#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <iostream>
using namespace std;

class CustomString {
public:
    // Function to calculate the length of a string
    static int custom_strlen(const char* str) {
        int length = 0;
        while (str[length] != '\0') {
            length++;
        }
        return length;
    }

    // Function to copy one string into another
    static void custom_strcpy(char* dest, const char* src) {
        int i = 0;
        while (src[i] != '\0') {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    // Function to concatenate two strings
    static void custom_strcat(char* dest, const char* src) {
        int dest_len = custom_strlen(dest);
        int i = 0;
        while (src[i] != '\0') {
            dest[dest_len + i] = src[i];
            i++;
        }
        dest[dest_len + i] = '\0';
    }

    // Function to compare two strings
    static int custom_strcmp(const char* str1, const char* str2) {
        int i = 0;
        while (str1[i] != '\0' && str2[i] != '\0') {
            if (str1[i] != str2[i]) {
                return str1[i] - str2[i];
            }
            i++;
        }
        return str1[i] - str2[i];
    }

    // Function to reverse a string
    static void custom_strrev(char* str) {
        int len = custom_strlen(str);
        for (int i = 0; i < len / 2; i++) {
            char temp = str[i];
            str[i] = str[len - i - 1];
            str[len - i - 1] = temp;
        }
    }

    // Function to find the first occurrence of a character in a string
    static char* custom_strchr(char* str, char c) {
        while (*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return nullptr;
    }


};

#endif // CUSTOM_STRING_H
