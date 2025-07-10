#ifndef PRIMERO_H
#define PRIMERO_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QList>
#include "Segundo.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class primero;
    // Nombre exacto que debe coincidir con el .ui
}
QT_END_NAMESPACE

class Primero : public QDialog  // Nombre de la clase principal
{
    Q_OBJECT

public:
    void extracted();
    explicit Primero(QWidget *parent = nullptr);
    ~Primero();
    void iniciar(); // Método para iniciar el juego

protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void continuarYSalto();      // función combinada
    void actualizarSalto();
    void detectarColisionConNubes();
    void on_btnContinuar_clicked();
    void reiniciarNivel();
    void subirNivel();
    void nivelCompletado();

signals:
    void juegoTerminado();  // Señal cuando el juego finaliza

private:
    Ui::primero *ui;  // Coincide con el namespace Ui
    segundo *juego = nullptr;
    QPixmap fondoprimero;
    QTimer *timerSalto;
    int velocidadY;
    int velocidadX;
    int posicionInicialY;
    bool subiendo;
    QList<QLabel*> nubes;
};
#endif // PRIMERO_H
