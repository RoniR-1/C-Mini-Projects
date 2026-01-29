char* ft_strrchr(const char* str, int ch) {
    char *result = 0;
    int i = 0;
    while (str[i] != 0) {
        if (str[i] == ch) {
            result = (char *)&str[i]; 
        }
        i++;
    }
    if (ch == 0) {
        result = (char *)&str[i]; 
    }
    return result;
}