# Juego de la Serpiente en C

Este es un simple juego de la serpiente implementado en C. El objetivo del juego es controlar una serpiente que se mueve por la pantalla, comer comida para crecer y evitar chocar contra las paredes o su propio cuerpo.

## Requisitos

- Un compilador de C (como `gcc` en Linux o Windows con MinGW).
- Sistema operativo Linux para el soporte de las funciones específicas de terminal, como `gotoxy` y `kbhit`. Sin embargo, este código también debería funcionar en Windows con algunas modificaciones.

## Descripción del Juego

- **Objetivo**: Controlar la serpiente para que coma la comida (representada por 'X'), aumentando su tamaño a medida que avanza. Si la serpiente choca con las paredes o con su propio cuerpo, el juego termina.
- **Controles**: Utiliza las teclas de flechas (`↑`, `↓`, `←`, `→`) para mover la serpiente. 
- **Tecla de Pausa**: Pulsa la tecla `P` para pausar el juego. 
- **Tecla de Salida**: Pulsa la tecla `ESC` para salir del juego.

## Instrucciones

1. **Clonar o Descargar el Proyecto**:
   Si estás usando Git, puedes clonar el repositorio con el siguiente comando:
   ```bash
   git clone <url-del-repositorio>
