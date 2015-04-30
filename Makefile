http_handle: http_handler.c utils/curl_easy.c log/uulog.c
	gcc $^ -Ilog -lpthread -lcurl -o $@ 
