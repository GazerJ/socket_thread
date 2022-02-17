main: ui.o
	gcc -I ./include  -c  ./app/server/server.c  
	gcc  -o  main ./server.o ./ui.o ./db.o ./net.o
	rm *.o	
ui.o: db.o net.o 
	gcc -I ./include   -c ./lib/ui/ui.c		

db.o: 
	gcc -I ./include  -c ./lib/db/db.c


net.o: 
	gcc -I ./include  -c ./lib/net/net.c

.PHONY:clean
clean:
	rm main
