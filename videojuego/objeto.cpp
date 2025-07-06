#include "objeto.h"
#include <QRandomGenerator>

Objeto::Objeto(QWidget *parent, int danio) : QLabel(parent), danio(danio) {
    QPixmap img("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/piedra.jpeg");  // Imagen de roca (o cualquier objeto dañino)
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
