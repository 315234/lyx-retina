/*****************************************************
 * provides strerror()
 * author Stephan Witt <stephan.witt@beusen.de>
 *****************************************************/

#include <config.h>

/*	$Id: strerror.c,v 1.3 2000/08/04 13:12:30 lasgouttes Exp $	*/

#if !defined(lint) && !defined(WITH_WARNINGS)
static char vcid[] = "$Id: strerror.c,v 1.3 2000/08/04 13:12:30 lasgouttes Exp $";
#endif /* lint */

extern	int	sys_nerr ;
extern	char *	sys_errlist [] ;

char * strerror (int errnum)
{
	static	char *	errtext = "unknown errno" ;

	if ( errnum < sys_nerr )
		return sys_errlist [errnum] ;
	return errtext ;
}
