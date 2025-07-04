#include "primero.h"
#include "ui_primero.h"  // Nombre debe coincidir con el .ui
#include <QApplication>
#include <QPixmap>
#include <QRandomGenerator>
Primero::Primero(QWidget *parent)
    : QDialog(parent), ui(new Ui::primero)
{
    ui->setupUi(this);
    {
        qDebug() << "Construyendo Primero...";

        try {
            ui->setupUi(this);
            qDebug() << "UI configurada correctamente";

            // Carga segura de imagen
            if (!fondoprimero.load("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fondoprimero.jpeg")) {
                qWarning() << "No se pudo cargar la imagen. Usando color sólido";
                fondoprimero = QPixmap(size());
                fondoprimero.fill(Qt::darkBlue);
            }
        } catch (...) {
            qCritical() << "Error durante la construcción";
            throw;
        }
    }
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(fondoprimero));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    qDebug() << "Tamaño de fondo:" << fondoprimero.size();
    qDebug() << "Tamaño de ventana:" << this->size();
}
void Primero::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // Siempre llamar primero a la clase base

    if (!fondoprimero.isNull()) {
        QPixmap fondoEscalado = fondoprimero.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QPalette pal;
        pal.setBrush(QPalette::Window, fondoEscalado);
        this->setPalette(pal);
    }
    QWidget::resizeEvent(event);
    QPixmap gokuImg(":/images/goku.png");
    if (!gokuImg.isNull()) {
        ui->gokuLabel->setPixmap(gokuImg.scaled(200, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->gokuLabel->setText("Goku aquí (imagen no encontrada)");
    }

    // Conectar botón a la función
    connect(ui->btnContinuar, &QPushButton::clicked, this, &MainWindow::continuarSimulacion);
}
Primero::~Primero()
{
    delete ui;
}
void Primero::iniciar() {
    qDebug() << "Juego iniciado!";
    // Lógica de inicio (cargar recursos, resetear puntuación, etc.)
}
void Primero::continuarSimulacion() {
    // Generar clima aleatorio
    int clima = QRandomGenerator::global()->bounded(3); // 0 a 2

    QString estado, mensaje;

    switch (clima) {
    case 0:
        estado = "🌤️ El cielo está tranquilo...";
        mensaje = "Kamisama: Este es un buen momento para respirar y meditar.";
        break;
    case 1:
        estado = "💨 Viento cruzado...";
        mensaje = "Kamisama: Mantén la postura, el viento es fuerte hoy.";
        break;
    case 2:
        estado = "☁️ Nube movediza...";
        mensaje = "Kamisama: La nube no se quedará quieta, enfoca tu energía.";
        break;
    }

    ui->labelEstado->setText(estado);
    ui->labelKamisama->setText("🌥️ " + mensaje);
}
