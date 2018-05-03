#include "etcp.h"

char *program_name;

//------------ MAIN -----------------------------------------------
int main( int argc, char **argv )
{
    struct sockaddr_in peer;
    SOCKET s;

    INIT();

    set_address( argv[ 1 ], argv[ 2 ], &peer, "tcp" );

    s = socket( AF_INET, SOCK_STREAM, 0 );
    if ( !isvalidsock( s ) )
	error( 1, errno, "ошибка вызова socket" );
    if ( connect( s, ( struct sockaddr * )&peer ,
	sizeof( peer ) ) )
	error( 1, errno, "ошибка вызова connect" );
    tcp_client( argv[ 1 ], argv[ 2 ] );
    EXIT( 0 );
}

/*----Функция set_address-------------------------------------------------

Для вызова данной функции напрямую необходимо использовать ее прототип:
#include "etcp.h"

void set_address( char *host, char *port,
    struct sockaddr_in *sap, char *protocol );
------------------------------------------------------------------------*/

    int set_address( char *hname, char *sname, 
	struct sockaddr_in *sap, char *protocol )
    {
	struct servent *sp;
	struct hostent *hp;
	char *endptr;
	short port;

	bzero( sap, sizeof( *sap ) );
	sap->sin_family = AF_INET;
	if ( hname != NULL )
	{
	    if ( !inet_aton( hname, &sap->sin_addr ) )
	    {
		hp = gethostbyname( hname );
		if ( hp == NULL )
		    error( 1, 0, "неизвестный хост: %s\n", hname );
		sap->sin_addr = *( struct in_addr * )hp->h_addr;
	    }
	}
	else
	    sap->sin_addr.s_addr = htonl( INADDR_ANY );
	port = strtol( sname, &endptr, 0 );
	if ( *endptr == '\0' )
	    sap->sin_port = htons( port );
	else
	{
	    sp = getservbyname( sname, protocol );
	    if ( sp == NULL )
		error( 1, 0, "неизвестный сервис: %s\n", sname );
	    sap->sin_port = sp->s_port;
	}
    }

/*----Функция error-----------------------------------------------------
Для вызова данной функции напрямую необходимо использовать ее прототип:
#include "etcp.h"

void error( int status, int err, char *format, ...);
----------------------------------------------------------------------*/

    void error( int status, int err, char *fmt, ... )
    {
	va_list ap;
	va_start( ap, fmt );
	fprintf( stderr, "%s: ", program_name );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
	if ( err )
	    fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
	if ( status )
	    EXIT( status );
    }
//---------------------------------------------------------
SOCKET tcp_client( char *hname, char *sname )
{
    struct sockaddr_in local;
    SOCKET s;
    const int on = 1;

    set_address( hname, sname, &local, "tcp" );
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if ( !isvalidsock( s ) )
	error( 1, errno, "ошибка вызова socket" );

    if ( setsockopt( s, SOL_SOCKET, SO_REUSEADDR,
	( char * )&on, sizeof( on ) ) )
	error( 1, errno, "ошибка вывода setsockopt" );

    if ( bind( s, ( struct sockaddr * ) &local,
	sizeof( local ) ) )
	error( 1, errno, "ошибка вызова bind ");

    if ( listen( s, NLISTEN ) )
	error( 1, errno, "ошибка вызова Listen" );

    return s;
}
//-----------------------------------------------------