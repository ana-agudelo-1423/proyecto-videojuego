#include "primero.h"
#include "ui_primero.h"  // Nombre debe coincidir con el .ui
#include <QApplication>
#include <QPixmap>
#include <QRandomGenerator>
Primero::Primero(QWidget *parent)
    : QDialog(parent), ui(new Ui::primero)
{
    ui->setupUi(this);  // Solo una vez

    try {
        // Carga segura de imagen
        if (!fondoprimero.load("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fondoprimero.jpeg")) {
            qWarning() << "No se pudo cargar la imagen. Usando color sólido";
            fondoprimero = QPixmap(size());
            fondoprimero.fill(Qt::darkBlue);
        }

        // Configurar fondo solo una vez en el constructor
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondoprimero));
        this->setPalette(palette);
        this->setAutoFillBackground(true);

    } catch (...) {
        qCritical() << "Error durante la construcción";
        throw;
    }

    // Configurar estilo de botón
    QString estiloBoton = R"(
        QPushButton {
            background-color: rgb(111, 194, 70);
            color: white;
            border-radius: 12px;
            padding: 8px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: rgb(163, 210, 110);
        }
        QPushButton:pressed {
            background-color: rgb(220, 130, 20);
        }
    )";
    this->setStyleSheet(estiloBoton);

    // Configurar imagen de Goku
    QPixmap gokuImg("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/goku.png");
    if (!gokuImg.isNull()) {
        ui->gokuLabel->setPixmap(gokuImg.scaled(111, 131, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->gokuLabel->setText("Goku aquí (imagen no encontrada)");
    }

    // Conectar botón
    connect(ui->btnContinuar, &QPushButton::clicked, this, &Primero::continuarSimulacion);
}

void Primero::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // Solo una llamada

    // Si necesitas escalar el fondo al redimensionar, puedes hacerlo aquí
    if (!fondoprimero.isNull()) {
        QPixmap fondoEscalado = fondoprimero.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondoEscalado));
        this->setPalette(palette);
    }
}
Primero::~Primero()
{
    delete ui;
}
void Primero::iniciar() {

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
