#ifndef _STDARG_H_
#define _STDARG_H_

typedef char* va_list;

#define va_start(list, last) list = (char*) &last + sizeof( last )
#define va_arg(list, type) *( (type*) ((list += sizeof(type)) - sizeof(type)))
#define va_end(marker) marker = (va_list) 0;

#endif /* _STDARG_H_ */
