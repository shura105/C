#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main( void )
{
	struct sockaddr_in local;
	int s;
	int s1;
	int rc;
	char buf[ 5 ];
	
	local.sin_family = AF_INET;
	local.sin_port = htons( 7500 );
	local.sin_addr.s_addr = htonl( INADDR_ANY );

	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0 )
	{
		perror( "Ошибка вызова socket" );
		exit( 1 );
	}
	
	rc = bind( s, ( struct sockaddr * )&local, sizeof( local ) );
	if ( rc < 0 )
	{
		perror( "Ошибка вызова bind" );
		exit ( 1 );	
	}

	rc = listen( s, 5 );
	if ( rc )
	{
		perror( "Ошибка вызова listen" );
		exit( 1 );
	}

	s1 = accept( s, NULL, NULL );
	if ( s1 < 0)
	{
		perror( "Ошибка вызова accept" );
		exit( 1 );
	}

	rc = recv( s1, buf, 5, 0 );
	if (rc <= 0)
	{
		perror( "Ошибка вызова recv" );
		exit( 1 );
	}
	printf( "%c\n", buf[ 0 ] );
	printf( "%c\n", buf[ 1 ] );
	printf( "%c\n", buf[ 2 ] );
	rc = send( s1, "Hello!!!", 8, 0 );
	if ( rc <= 0 )
		perror( "Ошибка вызова send" );
	exit( 0 );
}


















































