#include <Arduino.h>

class Formatter {
public:
    template<class T>
    inline static String format(const char *format, const T &value) {
        char buffer[8];
        snprintf(buffer, sizeof(buffer), format, value);
        return String(buffer);
    }
};