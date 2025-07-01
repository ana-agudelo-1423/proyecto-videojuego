#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_Juego_clicked();

    void on_Informacion_clicked();
    void on_pushButton_clicked();

    void on_Salir_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Widget *ui;
    QPixmap fondo;
};
#endif // WIDGET_H
