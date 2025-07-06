#ifndef OBJETO_H
#define OBJETO_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class Objeto : public QLabel {
    Q_OBJECT
public:
    explicit Objeto(QWidget *parent = nullptr, int danio = 10);
    bool checkColision(const QRect &personajeRect);  // Verifica colisión con Goku
    int getDanio() const;  // Obtener el daño del objeto
private:
    int danio;
};

#endif // OBJETO_H
