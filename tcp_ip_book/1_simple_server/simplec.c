#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main( void )
{
	struct sockaddr_in peer;
	int s;
	int rc;
	char buf[ 8 ];

	peer.sin_family = AF_INET;
	peer.sin_port = htons( 7500 );
	peer.sin_addr.s_addr = inet_addr( "127.0.0.1" );

	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( s < 0)
	{
		perror( "Ошибка вызова socket" );
		exit( 1 );
	}

	rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
	if ( rc )
	{
		perror( "Ошибка вызова connect" );
		exit ( 1 );
	}
	
	rc = send( s, "Hello", 5, 0 );
	if ( rc <= 0 )
	{
		perror( "Ошибка вызова send" );
		exit( 1 );
	}
	
	rc = recv( s, buf, 8, 0);
	if ( rc <= 0)
		perror( "Ошибка вызова recv" );
	else
		printf("%c\n", buf[ 0 ] );
		printf("%c\n", buf[ 1 ] );
		printf("%c\n", buf[ 2 ] );
	exit( 0 );
}
