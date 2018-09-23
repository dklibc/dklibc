/*
"Usually this header is supplied with your C compiler - only it knows a 
stack layout". Hmmm... But what about C calling convention? Of course it 
depends on arch. May be they suppose that calling convention is legacy, 
since no one programming asm nowdays?
*/

#ifndef _STDARG_H
#define _STDARG_H

typedef char* va_list;

// Amount of space in stack for arg of type @t
#define __va_argsz(t) (((sizeof(t) + sizeof(int) - 1)/sizeof(int)) * sizeof(int))

/*
Start variable argument list processing by setting @ap to point to 
the argument after @top 
*/
#define va_start(ap, top) (ap = (char *)&top + __va_argsz(top))

// End processing of variable argument list. In this case do nothing
#define va_end(ap)

/*
Increment @ap to point to the next arg in the list while returning 
pointer to what @ap pointed to first, which is of type @t
*/
#define va_arg(ap, t) (ap = ap + __va_argsz(t), *(t *)(ap - __va_argsz(t)))

#endif

