#ifndef __SKEL_H__
#define __SKEL_H__

/*Версия для Unix*/

#define INIT()		( program_name = \
			strrchr( argv[ 0 ],'/' ) ) ? \
			program_name++ : \
			( program_name = argv[ 0 ] )
#define EXIT(s)		exit( s )
#define CLOSE(s)	if ( close( s ) ) error( 1, errno, \
			"ошибка close " )
#define set_errno(e)	errno = ( e )
#define isvalidsock(s)	( ( s ) >= 0 )

typedef int SOCKET;

#endif /* __SKEL_H__ */