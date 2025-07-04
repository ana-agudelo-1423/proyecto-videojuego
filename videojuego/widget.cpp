#include "widget.h"
#include "ui_widget.h"
#include "primero.h"
#include <QMessageBox>
#include <QDialog>
#include <unistd.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    fondo.load("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fondo.png");

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette paleta;
        paleta.setBrush(QPalette::Window, fondoEscalado);
        this->setAutoFillBackground(true);
        this->setPalette(paleta);
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cargar la imagen.");
    }
    QString estiloBoton = R"(
    QPushButton {
        background-color: rgb(255, 172, 60);
        color: white;
        border-radius: 12px;
        padding: 8px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: rgb(255, 150, 30);
    }

    QPushButton:pressed {
        background-color: rgb(220, 130, 20);
    }

)";

    this->setStyleSheet(estiloBoton);

}

Widget::~Widget()
{
    delete ui;
}
void Widget::abrirPrimero() {
    Primero *dialogo = new Primero(this);
    dialogo->setAttribute(Qt::WA_DeleteOnClose); // Autoeliminación al cerrar

    connect(dialogo, &QDialog::finished, this, [this](int result) {
        if (result == QDialog::Accepted) {
            qDebug() << "Juego aceptado";
        }
    });

    dialogo->exec();
}
void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // Siempre llamar primero a la clase base

    if (!fondo.isNull()) {
        QPixmap fondoEscalado = fondo.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QPalette pal;
        pal.setBrush(QPalette::Window, fondoEscalado);
        this->setPalette(pal);
    }
    QWidget::resizeEvent(event);

    // Obtener tamaño actual
    int w = this->width();
    int h = this->height();

    // Reposicionar botones manualmente en proporción
    ui->Juego->move(w * 0.4, h * 0.55);
    ui->Informacion->move(w * 0.4, h * 0.68);
    ui->Salir->move(w * 0.4, h * 0.8);
}

void Widget::on_Informacion_clicked(){

    QMessageBox msgBox;
    msgBox.setText("Informacion de niveles.");
    msgBox.exec();
}
void Widget::on_Juego_clicked()
{
    qDebug() << "Intentando abrir juego...";

    if (!juego) {
        qDebug() << "Creando nueva instancia de Primero";
        juego = new Primero(this);

        // Verificación crítica
        if (!juego) {
            qCritical() << "Fallo al crear instancia de Primero";
            return;
        }
    }

    try {
        qDebug() << "Iniciando juego...";
        juego->iniciar();
        juego->show();
        qDebug() << "Ventana mostrada. Visible?" << juego->isVisible();
    } catch (const std::exception& e) {
        qCritical() << "Excepción:" << e.what();
    } catch (...) {
        qCritical() << "Excepción desconocida";
    }
}

void Widget::on_Salir_clicked()
{
         this->close();
}

