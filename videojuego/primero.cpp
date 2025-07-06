#include "primero.h"
#include "segundo.h"
#include "ui_primero.h"  // Nombre debe coincidir con el .ui
#include <QApplication>
#include <QPixmap>
#include <QRandomGenerator>
#include <QTimer>
Primero::Primero(QWidget *parent)
    : QDialog(parent), ui(new Ui::primero)
{
    ui->setupUi(this);  // Solo una vez
    timerSalto = new QTimer(this);


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
            background-color: rgb(171, 213, 151 );
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
    connect(ui->btnSaltar, &QPushButton::clicked, this, &Primero::continuarYSalto);
    timerSalto = new QTimer(this);
    connect(timerSalto, &QTimer::timeout, this, &Primero::actualizarSalto);
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
void Primero::continuarYSalto() {
    if (timerSalto->isActive())
        return; // evita saltos múltiples

    // --- 1. CLIMA ALEATORIO ---
    int clima = QRandomGenerator::global()->bounded(3); // 0-2
    QString estado, mensaje;

    switch (clima) {
    case 0:
        estado = "🌤️ Cielo despejado...";
        mensaje = "Kamisama: Concéntrate en tu energía interior.";
        break;
    case 1:
        estado = "💨 Viento cruzado...";
        mensaje = "Kamisama: Mantente firme, incluso con el viento.";
        break;
    case 2:
        estado = "☁️ Nube movediza...";
        mensaje = "Kamisama: La nube es inestable, confía en tu instinto.";
        break;
    }

    ui->labelEstado->setText(estado);
    ui->labelKamisama->setText("🌥️ " + mensaje);

    // --- 2. INICIAR SALTO ---
    posicionInicialY = ui->gokuLabel->y();
    velocidadY = -10;
    velocidadX = 3;
    subiendo = true;
    timerSalto->start(16);

}
void Primero::actualizarSalto() {
    int x = ui->gokuLabel->x();
    int y = ui->gokuLabel->y();

    y += velocidadY;
    x += velocidadX;

    ui->gokuLabel->move(x, y);

    if (subiendo) {
        if (velocidadY < 0)
            velocidadY++;
        else
            subiendo = false;
    } else {
        velocidadY++;
        if (y >= posicionInicialY) {
            ui->gokuLabel->move(x, posicionInicialY);
            timerSalto->stop();
        }
    }
}
void Primero::on_btnContinuar_clicked() {
    if (!juego) {
        juego = new segundo(nullptr);  // Sin padre
    }
    juego->show();
    this->hide();  // Opcional: ocultar Primero
    juego = new segundo(this);
    this->hide();  // Oculta Primero
    juego->show();
}

