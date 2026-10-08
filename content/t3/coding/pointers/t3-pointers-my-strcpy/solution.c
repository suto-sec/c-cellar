void my_strcpy(char *dst, const char *src)
{
    while ((*dst++ = *src++) != '\0')
        ;
}
