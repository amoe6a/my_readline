#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#define READLINE_READ_SIZE 5
unsigned long my_strlen(const char* arr)
{
    int lenght = 0;
    while (arr[lenght] != '\0' && arr[lenght]) lenght++;
    return lenght;
}
char* my_strcpy(char* param_1, const char* param_2)
{
    int i;
    for (i = 0; i < (int)my_strlen(param_2); i++) param_1[i] = param_2[i];
    param_1[my_strlen(param_2)] = '\0';
    return param_1;
}
char* my_strncpy(char* param_1, const char* param_2, int size)
{
    int i;
    for (i = 0; i < size; i++) param_1[i] = param_2[i];
    param_1[size] = '\0';
    return param_1;
}
char* my_char_memset(char* arr, int c, unsigned long len)
{
    char* dst = arr;
    while (len > 0)
    {
        *dst = (unsigned char)c;
        dst++;
        len--;
    }
    return arr;
}
char* my_strcat(char* dst, const char* src)
{
    int end = 0;
    while (dst[end] != '\0') end++;
    my_strcpy(&dst[end], src);
    return dst;
}

int find_newline(char* str)
{
    if (my_strlen(str) == 0 || str == NULL) return -1;
    int newlindx = 0;
    while (str[newlindx] != '\0')
    {
        if (str[newlindx] == '\n') return newlindx;
        newlindx++;
    }
    return -1;
}
char* my_readline_rec(int fd, char* last, char* remainder)
{
    unsigned long newsize = my_strlen(last) + my_strlen(remainder) + READLINE_READ_SIZE + 1;
    char buff[READLINE_READ_SIZE + 1];
    my_char_memset(buff, '\0', READLINE_READ_SIZE + 1);
    last = realloc(last, newsize);
    if (find_newline(remainder) >= 0)
    {
        my_strncpy(last, remainder, find_newline(remainder) + 1);
        my_strcpy(buff, &remainder[find_newline(remainder) + 1]);
        my_char_memset(remainder, '\0', READLINE_READ_SIZE + 1);
        my_strcpy(remainder, buff);
        return last;
    }
    while (read(fd, buff, READLINE_READ_SIZE) > 0)
    {
        if (find_newline(buff) >= 0)
        {
            if (my_strlen(remainder)) my_strcpy(last, remainder);
            my_char_memset(remainder, '\0', READLINE_READ_SIZE + 1);
            my_strcpy(remainder, &buff[find_newline(buff) + 1]);
            buff[find_newline(buff) + 1] = '\0';
            my_strcat(last, buff);
            return last;
        }
        else
        {
            if (my_strlen(remainder)) my_strcpy(last, remainder);
            my_strcat(last, buff);
            my_char_memset(remainder, '\0', READLINE_READ_SIZE + 1);
            return my_readline_rec(fd, last, remainder);
        }
    }
    if (my_strlen(remainder)) my_strcpy(last, remainder);
    my_char_memset(remainder, '\0', READLINE_READ_SIZE);
    return last;
}
char* my_readline(int fd)
{
    static char remainder[READLINE_READ_SIZE + 1] = {0};
    char* last = (char*)malloc(1);
    *last = '\0';
    if (read(fd, last, 0) < 0)
    {
        last = NULL;
        return last;
    }
    char* returner = my_readline_rec(fd, last, remainder);
    if (*returner == '\0')
    {
        free(returner);
        return NULL;
    }
    else
        return returner;
}