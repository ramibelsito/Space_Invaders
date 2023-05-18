Space_Invaders: menu
	./Space_Invaders_App
#Vamoajugá dijo el kuni

ifdef RASPI
menu:
	cd 'VERSION FINAL'
	make

else
ifdef ALLEGRO


else	#Si hacemos "make" a secas, nos va a salir un msj diciendo cómo usar el switch
menu:
	gcc -Wall -g -o Space_Invaders_App 'VERSION FINAL'/instrucciones.c


endif

endif