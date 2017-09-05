#include <stdio.h>
#include <stdarg.h>

int func_log(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	
	return 0;
}

#define debug_log(format, ...) func_log("[%s:%d] " format, __FUNCTION__, __LINE__, ##__VA_ARGS__);

int main()
{
	debug_log("hello\n");
	return 0;
}
