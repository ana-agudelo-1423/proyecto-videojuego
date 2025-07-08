#ifndef SEGUNDO_H
#define SEGUNDO_H
#include <QVector>
#include <QWidget>
#include "objeto.h"
#include "tercero.h"
#include <QProgressBar>
// Incluir la clase generada por Qt
namespace Ui {
class segundo;  // Declaración adelantada
}

class segundo : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
public:
  explicit segundo(QWidget *parent = nullptr);
    ~segundo();
    int iniciar();
   void moverGoku(int dx, int dy);
  void reducirVida(int cantidad);
private slots:


    void on_btnContinuar_clicked();

private:
    Ui::segundo *ui;  // Puntero a la interfaz de usuario
    tercero *juego = nullptr;
     QPixmap fondosegundo;
     QVector<Objeto*> objetos;
    void generarObjetos(int cantidad);
    void verificarColisiones();
    QProgressBar *barraVida;  // Barra de vida
    int vidaActual;

};

#endif // SEGUNDO_H
