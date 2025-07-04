#ifndef PRIMERO_H
#define PRIMERO_H

#include <QDialog>
#include <QPixmap>
QT_BEGIN_NAMESPACE
namespace Ui {
class primero;  // Nombre exacto que debe coincidir con el .ui
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
    void continuarSimulacion();
signals:
    void juegoTerminado();  // Señal cuando el juego finaliza

private:
    Ui::primero *ui;  // Coincide con el namespace Ui
    QPixmap fondoprimero;
};
#endif // PRIMERO_H
