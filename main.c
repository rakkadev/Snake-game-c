#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#define TAMANO_ARRAY_SERPIENTE 310

// Bibliotecas de Linux
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// Constantes de Linux
// Controles (teclas de dirección para Manjaro)
#define TECLA_ARRIBA  'A'
#define TECLA_IZQUIERDA  'D'
#define TECLA_DERECHA  'C'
#define TECLA_ABAJO 'B'
#define TECLA_ENTER 10

const char CABEZA_SERPIENTE = '@';
const char CUERPO_SERPIENTE = '*';
const char PARED = '|';
const char TECHO = '-';
const char COMIDA = 'X';
const char VACIO = ' ';

// Funciones de Linux - Estas emulan algunas funciones de la cabecera conio solo para Windows
void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

char getch()
{
    char c;
    system("stty raw");
    c = getchar();
    system("stty sane");
    return(c);
}

void clrscr()
{
    system("clear");
    return;
}
// Fin funciones Linux

// Esto debería ser igual en ambos sistemas operativos
#define BOTON_SALIR 27 // ESC
#define BOTON_PAUSA 112 // P

char esperarCualquierTecla(void)
{
int teclaPresionada;

while(!kbhit());

teclaPresionada = getch();
return((char)teclaPresionada);
}

int obtenerVelocidadJuego()
{
int velocidad = 1;
clrscr();

while(velocidad < 1 || velocidad > 9){
        return(velocidad);
}
return 1;
}

// Esta función revisa si se ha presionado una tecla, luego revisa si es alguna de las teclas de dirección/p/esc. Cambia la dirección según la tecla presionada.
int revisarTeclasPresionadas(int direccion)
{
int teclaPresionada;

if(kbhit()) // Si se presionó una tecla
{
    teclaPresionada = getch();
    if (direccion != teclaPresionada)
    {
        if(teclaPresionada == TECLA_ABAJO && direccion != TECLA_ARRIBA)
            direccion = teclaPresionada;
        else if(teclaPresionada == TECLA_ARRIBA && direccion != TECLA_ABAJO)
            direccion = teclaPresionada;
        else if(teclaPresionada == TECLA_IZQUIERDA && direccion != TECLA_DERECHA)
            direccion = teclaPresionada;
        else if(teclaPresionada == TECLA_DERECHA && direccion != TECLA_IZQUIERDA)
            direccion = teclaPresionada;
    }
}
return(direccion);
}

// Recorre y verifica si las coordenadas x, y corresponden a alguna parte de la serpiente
// Una serpiente de longitud 4 no puede colisionar consigo misma, por lo que no es necesario llamar a esta función si la longitud de la serpiente es <= 4
int colisionSerpiente (int x, int y, int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int longitudSerpiente, int detectar)
{
int i;
for (i = detectar; i < longitudSerpiente; i++) // Verifica si la serpiente colisionó consigo misma
{
    if ( x == serpienteXY[0][i] && y == serpienteXY[1][i])
        return(1);
}
return(0);
}

// Genera comida y se asegura de que no aparezca encima de la serpiente (¡esto a veces causa un problema de lag! Aunque no es un problema grave)
int generarComida(int comidaXY[], int ancho, int alto, int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int longitudSerpiente)
{

do
{
    srand ( time(NULL) );
    comidaXY[0] = rand() % (ancho-2) + 2;
    srand ( time(NULL) );
    comidaXY[1] = rand() % (alto-6) + 2;
} while (colisionSerpiente(comidaXY[0], comidaXY[1], serpienteXY, longitudSerpiente, 0)); // Esto debería evitar que la "Comida" se cree encima de la serpiente. Sin embargo, la comida puede generarse encima de la serpiente, en ese caso, la serpiente debería comerla...

gotoxy(comidaXY[0], comidaXY[1]);
printf("%c", COMIDA);

return(0);
}

/*
Mueve el arreglo de la serpiente hacia adelante, es decir:
Esto:
x 1 2 3 4 5 6
y 1 1 1 1 1 1
Se convierte en esto:
x 1 1 2 3 4 5
y 1 1 1 1 1 1

Luego, dependiendo de la dirección (en este caso oeste - izquierda) se convierte en:

x 0 1 2 3 4 5
y 1 1 1 1 1 1

serpienteXY[0][0]--; <- si la dirección es izquierda, resta 1 a la coordenada x
*/
void moverArregloSerpiente(int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int longitudSerpiente, int direccion)
{
int i;
for( i = longitudSerpiente-1; i >= 1; i-- )
{
    serpienteXY[0][i] = serpienteXY[0][i-1];
    serpienteXY[1][i] = serpienteXY[1][i-1];
}

/*
Como no sabemos las nuevas coordenadas x, y de la cabeza de la serpiente,
tenemos que revisar la dirección y sumarle o restarle dependiendo de la dirección.
*/
switch(direccion)
{
    case TECLA_ABAJO:
        serpienteXY[1][0]++;
        break;
    case TECLA_DERECHA:
        serpienteXY[0][0]++;
        break;
    case TECLA_ARRIBA:
        serpienteXY[1][0]--;
        break;
    case TECLA_IZQUIERDA:
        serpienteXY[0][0]--;
        break;
}

return;
}

void mover(int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int longitudSerpiente, int direccion)
{
int x;
int y;

// Eliminar la cola (¡DEBE HACERSE ANTES DE MOVER EL ARREGLO!)
x = serpienteXY[0][longitudSerpiente-1];
y = serpienteXY[1][longitudSerpiente-1];

gotoxy(x, y);
printf("%c", VACIO);

// Cambiar la cabeza de la serpiente por una parte del cuerpo
gotoxy(serpienteXY[0][0], serpienteXY[1][0]);
printf("%c", CUERPO_SERPIENTE);

moverArregloSerpiente(serpienteXY, longitudSerpiente, direccion);

gotoxy(serpienteXY[0][0], serpienteXY[1][0]);
printf("%c", CABEZA_SERPIENTE);

gotoxy(1, 1); // Elimina el guion bajo intermitente.

return;
}

// Esta función verifica si la cabeza de la serpiente está sobre la comida, si es así genera más comida...
int comerComida(int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int comidaXY[])
{
if (serpienteXY[0][0] == comidaXY[0] && serpienteXY[1][0] == comidaXY[1])
{
    comidaXY[0] = 0;
    comidaXY[1] = 0; // Esto debería evitar un error desagradable (bucles), hay que comprobar si el error sigue existiendo...

    printf("\7"); // Sonido
    return(1);
}

return(0);
}

int deteccionColision(int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int anchoConsola, int altoConsola, int longitudSerpiente )
{
int colision = 0;
if ((serpienteXY[0][0] == 1) || (serpienteXY[1][0] == 1) || (serpienteXY[0][0] == anchoConsola) || (serpienteXY[1][0] == altoConsola - 4)) // Verifica si la serpiente colisionó con la pared o consigo misma
    colision = 1;
else
    if (colisionSerpiente(serpienteXY[0][0], serpienteXY[1][0], serpienteXY, longitudSerpiente, 1)) // Si la serpiente colisionó con la pared, no tiene sentido verificar si colisionó consigo misma
        colision = 1;

return(colision);
}

void actualizarBarraDeInformacion(int puntuacion, int velocidad)
{
gotoxy(5, 23);
printf("Puntuacion: %d", puntuacion);

gotoxy(5, 24);
printf("Velocidad: %d", velocidad);

gotoxy(52, 23);
printf("Programadores: n/a");

gotoxy(52, 24);
printf("Version: 0.1");

return;
}


void pantallaFinDeJuego(void)
{
int x = 17, y = 3;

gotoxy(x, y++);
printf(":'######::::::'###::::'##::::'##:'########:\n");
gotoxy(x, y++);
printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
gotoxy(x, y++);
printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
gotoxy(x, y++);
printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
gotoxy(x, y++);
printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
gotoxy(x, y++);
printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
gotoxy(x, y++);
printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
gotoxy(x, y++);
printf(":......::::..:::::..::..:::::..::........::\n");
gotoxy(x, y++);
printf(":'#######::'##::::'##:'########:'########::'####:\n");
gotoxy(x, y++);
printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
gotoxy(x, y++);
printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
gotoxy(x, y++);
printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
gotoxy(x, y++);
printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
gotoxy(x, y++);
printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
gotoxy(x, y++);
printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
gotoxy(x, y++);
printf(":.......::::::...:::::........::..:::::..::....::\n");

esperarCualquierTecla();
clrscr(); // Limpiar la consola
return;
}

// Función que carga el entorno, la serpiente, etc.
void iniciarJuego( int serpienteXY[][TAMANO_ARRAY_SERPIENTE], int comidaXY[], int anchoConsola, int altoConsola, int longitudSerpiente, int direccion, int puntuacion, int velocidad)
{
int finJuego = 0;
clock_t tiempoFinal;

// CALCULANDO EL TIEMPO DE ESPERA SEGÚN LA VELOCIDAD
int tiempoEsperaMilis = CLOCKS_PER_SEC - (velocidad) * (CLOCKS_PER_SEC / 10);  // Establece el tiempo de espera correcto según la velocidad seleccionada
int puntuacionTemporal = 10 * velocidad;
int direccionAnterior;
int puedeCambiarDireccion = 1;

tiempoFinal = clock() + tiempoEsperaMilis;

do
{
    if(puedeCambiarDireccion)
    {
        direccionAnterior = direccion;
        direccion = revisarTeclasPresionadas(direccion);
    }

    if(direccionAnterior != direccion) // Solución temporal para evitar que la serpiente colisione consigo misma
        puedeCambiarDireccion = 0;

    if(clock() >= tiempoFinal) // se mueve según la velocidad del equipo
    {
        mover(serpienteXY, longitudSerpiente, direccion);
        puedeCambiarDireccion = 1;

        if(comerComida(serpienteXY, comidaXY))
        {
            generarComida(comidaXY, anchoConsola, altoConsola, serpienteXY, longitudSerpiente); // Genera más comida
            longitudSerpiente++;
            puntuacion += 10;

            if(puntuacion >= 10 * velocidad + puntuacionTemporal)
            {
                velocidad++;
                puntuacionTemporal = puntuacion;

                if(velocidad <= 9) // esto necesita arreglarse
                    tiempoEsperaMilis = tiempoEsperaMilis - (CLOCKS_PER_SEC / 10);
                else
                {
                    if(tiempoEsperaMilis >= 40) // Velocidad máxima (el juego debe ser jugable)
                        tiempoEsperaMilis = tiempoEsperaMilis - (CLOCKS_PER_SEC / 200);
                }
            }

            actualizarBarraDeInformacion(puntuacion, velocidad);
        }

        tiempoFinal = clock() + tiempoEsperaMilis;
    }

    finJuego = deteccionColision(serpienteXY, anchoConsola, altoConsola, longitudSerpiente);
} while(finJuego == 0);

pantallaFinDeJuego(); 
return;
}

int main()
{
int anchoConsola = 80;
int altoConsola = 25;
int direccion = TECLA_DERECHA;
int longitudSerpiente = 5;
int serpienteXY[2][TAMANO_ARRAY_SERPIENTE] = {0}; // Esto debe ser inicializado correctamente
int comidaXY[2] = {0}; // Comida posición
int puntuacion = 0;
int velocidad = 1; // Velocidad inicial

// INICIALIZANDO EL JUEGO
longitudSerpiente = 5;  // Establece el largo de la serpiente
serpienteXY[0][0] = 10;
serpienteXY[1][0] = 10;
serpienteXY[0][1] = 9;
serpienteXY[1][1] = 10;
serpienteXY[0][2] = 8;
serpienteXY[1][2] = 10;
serpienteXY[0][3] = 7;
serpienteXY[1][3] = 10;
serpienteXY[0][4] = 6;
serpienteXY[1][4] = 10;

generarComida(comidaXY, anchoConsola, altoConsola, serpienteXY, longitudSerpiente); // Crea la comida

// Llamada principal para comenzar el juego
iniciarJuego(serpienteXY, comidaXY, anchoConsola, altoConsola, longitudSerpiente, direccion, puntuacion, velocidad);

return 0;
}

