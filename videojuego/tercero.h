#ifndef TERCERO_H
#define TERCERO_H
#include "qprogressbar.h"
#include <QWidget>
#include <QPixmap>
#include <QRandomGenerator>
#include <QLabel>
#include <QList>
#include <QString>
#include <QProgressBar>
#include <QPushButton>
namespace Ui {
class tercero;  // Declaración adelantada
}
class Personaje {
private:
    QString nombre;
    int vida;
    int ataque;
    int defensa;
    int vidaMaxima;

public:
    Personaje(QString n, int v, int a, int d) : nombre(n), vida(v), ataque(a), defensa(d) {}

    bool atacar(Personaje &oponente) {
        int danio = qMax(1, ataque - (oponente.defensa / 2));
        danio += QRandomGenerator::global()->bounded(5) - 2;
        if (danio < 1) danio = 1;

        oponente.vida -= danio;

        if (oponente.vida <= 0) {
            return true;
        }
        return false;
    }

    QString getNombre() const { return nombre; }
    int getVida() const { return vida; }
    int getVidaMaxima() const { return vidaMaxima; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }
    int danio();
    void recibirDanio(int cantidad);
};

class tercero : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    explicit tercero(QWidget *parent = nullptr);
    ~tercero();
private slots:
    void iniciarPelea();
    void turnoJugador();
    void turnoEnemigo();
    void actualizarUI();
    void resetearJuego();
    void lanzarBolaDeEnergia();
    void moverBolas();
    void on_btnSalir_clicked();
    void lanzarBolaDeEnergiaEnemigo();
    void on_btnAtaque_clicked();


private:
    QPixmap fondotercero;
    Ui::tercero *ui;
    Personaje *jugador;
    Personaje *enemigo;
    QTimer *timer;
    bool turnoActual;
    QList<QLabel*> bolasEnergia;  // Guardará todas las bolas activas
    QTimer* timerBolas;           // Timer para moverlas
    QProgressBar *vidaGoku;
    QProgressBar *vidaPiccolo;
    struct BolaEnergia {
        QLabel* visual;
        bool esEnemiga;
        QPoint velocidad;
        QPixmap textura;
    };
};
#endif // TERCERO_H
