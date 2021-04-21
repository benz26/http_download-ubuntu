OBJS =  main.o \
		download_file.o \
		download_conn_http.o  \
		download_md5.o \
		download_thread.o \
		download_conn.o
		
INCLUDE =   download_utils.h \
			download_file.h \
			download_conn.h \
			download_conn_http.h \
			download_md5.h \
			download_thread.h
			
LIBS  = -lcurl -lcrypto -lssl 
CC = g++
CFLAG = -Wall -pthread  -g   


a.out : $(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o $@ $(LIBS)

%.o: %.c download_utils.h
	$(CC) $(CFLAG) -c $< -o $@   
	

main.o:	$(INCLUDE)

download_conn.o download_file.o download_conn_http.o : download_conn.h

download_file.o download_thread.o download_conn_http.o : download_file.h

download_file.o download_thread.o: download_thread.h

download_conn_http.o : download_conn_http.h

download_md5.o : download_md5.h


all:*.cpp *.h
	$(CC) $(CFLAG)  *.cpp -o $@ -lcurl -lcrypto -lssl 	


.PHONY: clean  
clean:  
	rm -rf $(OBJS) a.out  all
