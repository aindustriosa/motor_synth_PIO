#include "serial.h"


namespace motor_synth
{
    class MockSerialIO : public SerialIO
    {
    public:
        void print(const char *text)
        {
            printf(text);
        }
        size_t print(int n, int base)
        {
            return printf("%d", n);
        }
        size_t println(unsigned int n, int base)
        { 
            return printf("%d\n", n);
        }
        size_t println()
        {
            return printf("\n");
        }
        size_t println(const char *text)
        {
            size_t res = printf(text);
            printf("\n");
            return res;
        }
        void println(const char *text, int number)
        {
            printf(text);
            printf("%d\n", number);
        }
    };
}