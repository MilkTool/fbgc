#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#define _FBGC_NO_ERROR 0
#define _FBGC_LEXER_ERROR 1
#define _FBGC_SYNTAX_ERROR 2
#define _FBGC_UNDEFINED_IDENTIFIER_ERROR 3
#define _FBGC_DUMMY2_ERROR 4



#define ANSI_COLOR_BLACK    "\033[1;30m" //000
#define ANSI_COLOR_RED      "\033[1;31m" //100
#define ANSI_COLOR_GREEN    "\033[1;32m" //010
#define ANSI_COLOR_YELLOW   "\033[1;33m" //011
#define ANSI_COLOR_BLUE     "\033[1;34m" //001
#define ANSI_COLOR_MAGENTA  "\033[1;35m" //101
#define ANSI_COLOR_CYAN     "\033[1;36m" //011
#define ANSI_COLOR_WHITE    "\033[1;37m" //111
#define ANSI_COLOR_RESET    "\033[0m" //0

#define ANSI_COLOR_RED_UNDERLINE    "\e[4;31m"

#define LOG_LEVEL_ERROR     0
#define LOG_LEVEL_WARNING   1
#define LOG_LEVEL_DEBUG     2
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_VERBOSE   4

#ifdef LOG_LEVEL 
    #define LOG_LEVEL LOG_LEVEL_ERROR
#else
    #define LOG_LEVEL LOG_LEVEL_VERBOSE
#endif


#define FBGC_LOGFORMATSIMPLE(color,format) color "" format "\n"ANSI_COLOR_RESET 
#define FBGC_LOGFORMATDETAILED(color,format) color "%s function:%s at line %u: " format "\n"ANSI_COLOR_RESET ,__FILE__,__FUNCTION__,  __LINE__


#ifdef LOG_LEVEL >= LOG_LEVEL_ERROR
	#define FBGC_LOGE(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_RED,format),##__VA_ARGS__)
	#define _FBGC_LOGE(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_RED,format),##__VA_ARGS__)
#else
	#define FBGC_LOGE(...)
	#define _FBGC_LOGE(...)
#endif

#ifdef LOG_LEVEL >= LOG_LEVEL_WARNING
	#define FBGC_LOGW(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_YELLOW,format),##__VA_ARGS__)
	#define _FBGC_LOGW(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_YELLOW,format),##__VA_ARGS__)
#else
	#define FBGC_LOGW(...)
	#define _FBGC_LOGW(...)
#endif

#ifdef LOG_LEVEL >= LOG_LEVEL_DEBUG
	#define FBGC_LOGD(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_CYAN,format),##__VA_ARGS__)
	#define _FBGC_LOGD(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_CYAN,format),##__VA_ARGS__)
#else
	#define FBGC_LOGD(...)
	#define _FBGC_LOGD(...)
#endif

#ifdef LOG_LEVEL >= LOG_LEVEL_INFO
	#define FBGC_LOGI(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_WHITE,format),##__VA_ARGS__)
	#define _FBGC_LOGI(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_WHITE,format),##__VA_ARGS__)
#else
	#define FBGC_LOGI(...)
	#define _FBGC_LOGI(...)
#endif

#ifdef LOG_LEVEL >= LOG_LEVEL_VERBOSE
	#define FBGC_LOGV(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_MAGENTA,format),##__VA_ARGS__)
	#define _FBGC_LOGV(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_MAGENTA,format),##__VA_ARGS__)
#else
	#define FBGC_LOGV(...)
	#define _FBGC_LOGV(...)
#endif	



//#ifdef INFO_PRINT_AVAILABLE
#define _info(s,...)(cprintf(111,s,##__VA_ARGS__))
#define _info_green(s,...)(cprintf(010,s,##__VA_ARGS__))
#define _warning(s,...)(cprintf(110,s,##__VA_ARGS__))
#define _print(s,...)(cprintf(101,s,##__VA_ARGS__))
#define _cprint(c,s,...)(cprintf(c,s,##__VA_ARGS__))
#define _debug(s,...)(cprintf(011,s,##__VA_ARGS__))
#define _print_object(s,obj)(cprintf(001,s), print_fbgc_object(obj))
#define _println_object(s,obj)(cprintf(001,s), print_fbgc_object(obj),cprintf(001,"\n"))
#define _obj2str(obj)(object_name_array[obj->type])
#define _ll2str(obj)(object_name_array[obj->type])
#define _gm2str(gm)(gm_name_array[gm])

//#endif

#if defined CLOSE_PRINTF
	#define cprintf(...)
#else
	int cprintf(char color,const char *format, ...);
#endif

int fbgc_assert(unsigned char cond,const char * format,...);
int fbgc_error(unsigned char error_code,int line_no);
//#define LEXER_DEBUG
//#define GRAMMAR_DEBUG
//#define PARSER_DEBUG
//#define INTERPRETER_DEBUG


#ifdef  __cplusplus
}
#endif

#endif