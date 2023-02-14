#https://lucid.app/lucidchart/e59ea774-2036-47a0-825b-6e0b9c497927/edit?viewport_loc=-29%2C9597%2C1311%2C643%2C0_0&invitationId=inv_edba393d-c990-4689-9fc9-e2433e39c39c

#Para definir usamos -DALLEGRO o -DRASPI
#Al invocar el makefile haciendo "make", nos salen las instrucciones para saber cómo instalar cada versión

#-lallegro -lallegro_image -lallegro_font -lallegro_ttf
#Las librerías de allegro no se agregan como dependencias, sino como flags de compilación


Space_Invaders: menu
	./Space_Invaders_App
#Vamoajugá dijo el kuni


ifdef RASPI

menu:	standard.h menu.c player.h player.o configs.h configs.o entorno.o 
	gcc -Wall -g -o Space_Invaders_App menu.c configs.o player.o entorno.o dibujosRaspi.o disdrv.o joydrv.o -lpthread -DRASPI

configs.o: standard.h configs.h configs.c
	gcc -Wall -g -c configs.c -lpthread -DRASPI

player.o: standard.h player.h player.c entorno.h entorno.o joydrv.h disdrv.h dibujosRaspi.o
	gcc -Wall -g -c player.c entorno.o joydrv.o disdrv.o  dibujosRaspi.o -lpthread -DRASPI

entorno.o: standard.h entorno.h entorno.c
	gcc -Wall -g -c entorno.c -lpthread -DRASPI

dibujosRaspi.o: standard.h dibujosRaspi.h dibujosRaspi.c disdrv.h
	gcc -Wall -g -c dibujosRaspi.c disdrv.o -DRASPI


else
ifdef ALLEGRO

menu: standard.h menu.c player.h player.o configs.h configs.o
	gcc -Wall -g -o Space_Invaders_App menu.c player.o configs.o entorno.o -lpthread -lallegro -lallegro_image -lallegro_font -lallegro_ttf -DALLEGRO

configs.o: standard.h configs.h configs.c
	gcc -Wall -g -c configs.c -DALLEGRO

player.o: standard.h player.h player.c entorno.h entorno.o
	gcc -Wall -g -c player.c entorno.o -lpthread -lallegro -lallegro_image -lallegro_font -lallegro_ttf -DALLEGRO

entorno.o: standard.h entorno.h entorno.c
	gcc -Wall -g -c entorno.c -DALLEGRO


else	#Si hacemos "make" a secas, nos va a salir un msj diciendo cómo usar el switch
menu:
	gcc -Wall -g -o Space_Invaders_App instrucciones.c


endif

endif