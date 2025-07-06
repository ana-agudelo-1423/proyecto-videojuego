#ifndef PRIMERO_H
#define PRIMERO_H

#include <QDialog>
#include <QPixmap>
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
    explicit Primero(QWidget *parent = nullptr);
    ~Primero();
    void iniciar();  // Método para iniciar el juego

protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void continuarYSalto();      // función combinada
    void actualizarSalto();
    void on_btnContinuar_clicked();

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
};
#endif // PRIMERO_H
