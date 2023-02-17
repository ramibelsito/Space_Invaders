# Space-Invaders

![space_invaders_tittle (2)](https://user-images.githubusercontent.com/105401456/218676467-b196830e-d4ee-4631-95c8-edd46c96c267.png)

El juego tiene 5 niveles, pudiendo elegir como nivel de inicio a cualquiera de los tres primeros.
Es decir, para superar el nivel "hell" (nivel 5), sí o sí hay que superar "hard" y "hardcore", haciendo
que en cualquiera de las tres dificultades iniciales, el jugador tenga un sentimiento de 
progresión a medida que supera los distintos escenarios.
Una vez que se alcanza el nivel 5, en caso de superarlo, podrá repetirlo indefinidamente hasta 
perder para lograr así superar su récord.
Al perder o al reiniciar, la dificultad se setea en "defaultDifficulty" que, en caso de que el jugador
no haya seleccionado nada en el "difficultyMenu", la misma será "easy" (nivel 1).

Las barreras en ALLEGRO no están hardocedeadas. En función del ancho del display, la cantidad de 
barreras y el tamaño de ellas, las mismas se orden automáticamente.
Sin embargo, en RASPI, debido a que la misma cuenta únicamente con 16 píxeles de ancho, existe una
única disposición óptima en el acotado espacio disponible, así que optamos por hardcodearlas.

El proyecto cuenta con cuatro threads, uno que maneja el display (con switches de compilación), otro
que le da vida a los aliens, un tercero que desplaza las balas a través del display y un último
que se encarga de analizar las colisiones y actuar en consecuencia (sumar puntos, "matar" objetos, etc)

Podría hablarse, si se quiere, de un quinto thread, que en realidad se ejecuta en paralelo a los otros 
cuatro pero en el mainstream. El mismo se encarga de manejar al jugador, y tiene tres condiciones de
salida (y al salir del loop se encarga de "frenar" también a los otros cuatro).

La primer condición es que el jugador aprete "pausa", en cuyo caso, el entorno del jugador (barreras,
aliens, balas, etc) NO se resetea, de modo que al presionar PLAY nuevamente, todo siga exactamente 
igual (a menos que selecciones "reiniciar" después de pausar, obviamente).
La segunda, es que el jugador pierda todas sus vidas. En dicho caso, todo el entorno se resetea, tomando
como referencia a la "defaultDifficulty", de modo que al presionar PLAY, el juego inicie 
"desde cero".
La última condición, es que el jugador supere el nivel, es decir, que elimine a todos los aliens.
Este caso es similar al anterior, solo que al resetear el entorno, no se toma como referencia a la 
"defaultDifficulty", sino a la siguiente respecto a la que se acaba de superar (a menos que ya se haya
alcanzado el máximo nivel). Además, cabe destacar que en este caso NO se resetean las vidas ni el 
puntaje del jugador, permitiendo que superar niveles de forma consecutiva sea la mejor manera de 
romper tu propio récord.

Actualmente, existen 3 tipos de aliens, que aparecena diferentes distancias respecto al jugador.
Los de tipo 1 son los más alejados, y por lo tanto, los más difíciles de matar. Los de tipo 3, los 
más fáciles, y así.
En este momento, matar un alien de tipo 1 da 15 puntos, de tipo 2 da 10, y de tipo 3 da 5.
Además, neutralizar una bala de alien (es decir, hacerla impactar con la tuya para que no te pegue)
te suma 1 punto. Esta útlima no es una práctica que modifique considerablemente tu puntaje, pero es
de vital importancia en casos de desempate (por ejemplo, cuando dos jugadores matan la misma cantidad
de aliens).
Por último, existe un tipo especial de alien, la nave nodriza, cuya aparición es aleatoria. La misma
te entregará 100 puntos y un corazón extra si conseguís eliminarla. Pero cuidado, si pierdes las vidas
que te entrega la nave nodriza además también perderás 50 de los 100 puntos que te entregó.

En caso de que tu puntaje final sea mayor al de la base de datos local, el jugador podrá registrar su 
nuevo récord junto a un nick de cuatro caracteres. Así, su nombre y su marca personal aparecerán en el
top 3 al ingersar al menu de "highscores". Felicitaciones! :)

-- creditos al creador del lofi loop: Bernat Cucarella Sifre :)

GUIA DE INSTALACION EN WINDOWS:
1. Entrar a la Microsoft Store e instalar Windows Subsystem for Linux (wsl)
2. En el menu de Start de Windows buscar "Windows Features"
![image](https://user-images.githubusercontent.com/105401456/218674788-70625a72-3b6d-4fa7-a530-c7d8bc005787.png)
Luego reiniciar la computadora.
3. Abrir la comand prompt y correr los siguientes comandos:
"wsl --list --online" (para mostrar las distros disponibles)
"wsl --install <NOMBRE_DE_DISTRO"
4. Luego de instalar la distro correr los siguientes comandos:
"sudo apt update", "sudo apt upgrade", "sudo apt install git", "sudo apt install gcc", "sudo apt install make", 
"sudo apt install liballegro5-dev"
5. Clonar el repositorio con el comando: "git clone <LINK_AL_REPO>"
6. Se creara una carpeta llamada "Space_Invaders" donde se encuentran los archivos del programa.

GUIA DE EJECUCION:
1. Entrar a la carpeta donde se encuentran los archivos de codigo: "cd ./Space_Invaders/VERSION_FINAL/"
2. Correr el comando "make ALLEGRO=1"
Si ya se corrieron estos comandos se creara un archivo "Space_Invaders_App" que le permitira ejecutar el 
programa sin la necesidad de recompilarlo (simplemente ejecute: "./Space_Invaders_App").


