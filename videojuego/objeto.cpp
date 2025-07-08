#include "objeto.h"
#include <QRandomGenerator>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QString>
using namespace std;
class Personaje {
private:
    string nombre;
    int vida;
    int ataque;
    int defensa;

public:
    Personaje(string n, int v, int a, int d) : nombre(n), vida(v), ataque(a), defensa(d) {}

    bool atacar(Personaje &oponente) {
        int danio = max(1, ataque - (oponente.defensa / 2));
        danio += rand() % 5 - 2; // Variabilidad aleatoria (-2 a +2)
        if (danio < 1) danio = 1;

        oponente.vida -= danio;

        cout << nombre << " ataca a " << oponente.nombre
             << " y causa " << danio << " de danio!" << endl;


        if (oponente.vida <= 0) {
            cout << "\n¡" << oponente.nombre << " ha sido derrotado!" << endl;
            cout << "FIN DE PARTIDA" << endl;
            return true;
        }
        return false;
    }

    void mostrarEstado() const {
        cout << nombre << ": Vida " << vida << endl;
    }

    string getNombre() const { return nombre; }
    int getVida() const { return vida; }
};

void mostrarEstados(const Personaje &jugador, const Personaje &enemigo) {
    cout << "\n--- ESTADO ---" << endl;
    jugador.mostrarEstado();
    enemigo.mostrarEstado();
    cout << "--------------\n" << endl;
}
Objeto::Objeto(QWidget *parent, int danio) : QLabel(parent), danio(danio) {
    QPixmap img("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/piedra.png");  // Imagen de roca (o cualquier objeto dañino)
    if (!img.isNull()) {
        setPixmap(img.scaled(50, 50, Qt::KeepAspectRatio));
    } else {
        setText("💥");  // Fallback visual
    }
    move(QRandomGenerator::global()->bounded(300), 0);  // Posición aleatoria en la parte superior
}

int Objeto::getDanio() const {
    return danio;
}


bool Objeto::checkColision(const QRect &personajeRect) {
    return personajeRect.intersects(this->geometry());
}
int main() {
    srand(time(0)); // Inicializar semilla para números aleatorios

    Personaje jugador("Heroe", 100, 20, 10);
    Personaje enemigo("Villano", 80, 18, 8);

    cout << "¡COMIENZA LA PELEA!\n" << endl;

    while (true) {
        // Turno del jugador
        if (jugador.atacar(enemigo)) {
            break;
        }
        mostrarEstados(jugador, enemigo);


        // Turno del enemigo
        if (enemigo.atacar(jugador)) {
            break;
        }
        mostrarEstados(jugador, enemigo);

    }

    return 0;
}
