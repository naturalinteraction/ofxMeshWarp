
// (c) Alessandro Valli 2020

#ifndef say_h
#define say_h

#include <sys/stat.h>

inline bool file_exists(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

inline bool contains(string s1, string s2)
{
    return (s1.find(s2) != std::string::npos);
}

inline bool replaceSubstring(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

inline void replaceSubstringForever(std::string& str, const std::string& from, const std::string& to)
{
    string before;
    do
    {
        before = str;
        replaceSubstring(str, from, to);
    }
    while (before != str);
}

#define say(...)       printf("%s %s %s %s\n", timecode_string().c_str(), __func__, #__VA_ARGS__, form(__VA_ARGS__))
#define whisper(...)   printf("%s %s\n", timecode_string().c_str(), form(__VA_ARGS__))

#define NOT_REALLY_PROUD_OF_THIS_CONSTANT   8000

inline bool char_star_too_long(const char *str)
{
    if (strlen(str) > NOT_REALLY_PROUD_OF_THIS_CONSTANT * 0.5 - 256)
        return true;
    return false;
}

inline bool string_too_long(string &str)
{
    if (str.size() > NOT_REALLY_PROUD_OF_THIS_CONSTANT * 0.5 - 256)
        return true;
    return false;
}

inline string timecode_string()
{
    char global_timecode_string_buffer[255];

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(global_timecode_string_buffer, sizeof(global_timecode_string_buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    string result = string(global_timecode_string_buffer);
    return result;
}

inline const char * form()
{
    return "";
}

inline const char * form(const char *arg)
{
    return arg;
}

inline const char * form(float arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f", arg);
    return formatted;
}

inline const char * form(double arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f", arg);
    return formatted;
}

inline const char * form(int arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d", arg);
    return formatted;
}

inline const char * form(bool arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d", arg);
    return formatted;
}

inline const char * form(unsigned int arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d", arg);
    return formatted;
}

inline const char * form(size_t arg)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%ld", arg);
    return formatted;
}

inline const char * form(const char *str, int arg)
{
    if (char_star_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d", arg);
    return formatted;
}

inline const char * form(const char *str, unsigned int arg)
{
    if (char_star_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d", arg);
    return formatted;
}

inline const char * form(string str1, string str2)
{
    if (string_too_long(str1) || string_too_long(str2))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%s %s", str1.c_str(), str2.c_str());
    return formatted;
}

inline const char * form(string str, int num)
{
    if (string_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%s %d", str.c_str(), num);
    return formatted;
}

inline const char * form(string str)
{
    if (string_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%s", str.c_str());
    return formatted;
}

inline const char * form(const char *str, float arg)
{
    if (char_star_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f", arg);
    return formatted;
}

inline const char * form(int arg1, int arg2)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d %d", arg1, arg2);
    return formatted;
}

inline const char * form(float arg1, float arg2)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f %f", arg1, arg2);
    return formatted;
}

inline const char * form(double arg1, double arg2)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f %f", arg1, arg2);
    return formatted;
}

inline const char * form(int arg1, int arg2, int arg3, int arg4)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%d %d %d %d", arg1, arg2, arg3, arg4);
    return formatted;
}

inline const char * form(ofRectangle &r)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%.1f %.1f %.1f %.1f", r.x, r.y, r.width, r.height);
    return formatted;
}

inline const char * form(ofVec2f &v)
{
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%f %f", v.x, v.y);
    return formatted;
}

inline const char * form(string str, ofVec2f &v)
{
    if (string_too_long(str))
        return "string too long, can't say";
    static char formatted[NOT_REALLY_PROUD_OF_THIS_CONSTANT];
    sprintf(formatted, "%s %f %f", str.c_str(), v.x, v.y);
    return formatted;
}

#endif
