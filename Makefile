tcp_client: tcp_client.c
	gcc tcp_client.c -o tcp_client


tcp_server: tcp_server.c
	gcc tcp_server.c -o tcp_server

tcp_select_server: tcp_select_server.c
	gcc tcp_select_server.c -o tcp_select_server

clean:
	rm tcp_client tcp_server