#include "segundo.h"
#include "ui_segundo.h"
#include "objeto.h"// ¡Este include es crucial!
#include <QPixmap>
#include <QKeyEvent>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTimer>
segundo::segundo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::segundo)  // Inicializar el puntero ui
{
    ui->setupUi(this);  // Configurar la interfaz
    try {
        // Carga segura de imagen
        if (!fondosegundo.load("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fondosegundo.jpeg")) {
            qWarning() << "No se pudo cargar la imagen. Usando color sólido";
            fondosegundo = QPixmap(size());
            fondosegundo.fill(Qt::darkBlue);
        }

        // Configurar fondo solo una vez en el constructor
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondosegundo));
        this->setPalette(palette);
        this->setAutoFillBackground(true);

    } catch (...) {
        qCritical() << "Error durante la construcción";
        throw;
    }
    QString estiloBoton = R"(
        QPushButton {
            background-color: rgb(123, 36, 28 );
            color: white;
            border-radius: 12px;
            padding: 8px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: rgb(129, 27, 27  );
        }
        QPushButton:pressed {
            background-color: rgb(162, 0, 0 );
        }
    )";
    this->setStyleSheet(estiloBoton);
    QPixmap gokuImg("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/goku.png");
    if (!gokuImg.isNull()) {
        ui->gokuLabel->setPixmap(gokuImg.scaled(111, 131, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->gokuLabel->setText("Goku aquí (imagen no encontrada)");
    }
    this->setAttribute(Qt::WA_DeleteOnClose);  // Liberar memoria al cerrar
    this->setWindowModality(Qt::ApplicationModal);  // Opcional: ventana modal
generarObjetos(5);
    barraVida = new QProgressBar(this);
    barraVida->setGeometry(10, 10, 200, 20);  // Posición y tamaño (x, y, ancho, alto)
    barraVida->setRange(0, 100);              // Rango (0% a 100%)
    vidaActual = 100;
    barraVida->setValue(vidaActual);          // Valor inicial
    barraVida->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; }"
                             "QProgressBar::chunk { background-color: #FF0000; }");
    barraVida->setStyleSheet(R"(
    QProgressBar {
        border: 2px solid #333;
        border-radius: 5px;
        text-align: center;  /* Alinea el texto al centro */
        color: white;        /* Color del texto */
        font-weight: bold;
    }
    QProgressBar::chunk {
        background-color: #FF0000;  /* Color de la barra de progreso */
    })");

    // Mostrar el porcentaje como texto
    barraVida->setFormat("%p%");
    ui->btnContinuar->hide();
}
void segundo::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // Solo una llamada

    // Si necesitas escalar el fondo al redimensionar, puedes hacerlo aquí
    if (!fondosegundo.isNull()) {
        QPixmap fondoEscalado = fondosegundo.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondoEscalado));
        this->setPalette(palette);
    }
}
segundo::~segundo()
{
    delete ui;  // Liberar memoria
}
//segundo :: iniciar(){
//     return 0;
//}

void segundo::generarObjetos(int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        Objeto *obj = new Objeto(this);
        objetos.append(obj);  // ¡Sin errores!
        obj->show();
    }
}

void segundo::verificarColisiones() {
    QRect gokuRect = ui->gokuLabel->geometry();

    for (Objeto *obj : objetos) {
        if (obj->checkColision(gokuRect)) {
            reducirVida(obj->getDanio());  // Reduce la vida según el daño del objeto
            qDebug() << "¡Goku golpeado! Daño recibido:" << obj->getDanio();

            // Opción 1: El objeto desaparece después de golpear
            objetos.removeOne(obj);
            delete obj;

            // Opción 2: El objeto permanece (para daño continuo)
            // obj->move(QRandomGenerator::global()->bounded(width()), 0);  // Reubicar

            break;
        }
    }

}
void segundo::moverGoku(int dx, int dy) {
    ui->gokuLabel->move(ui->gokuLabel->x() + dx, ui->gokuLabel->y() + dy);
    verificarColisiones();  // Verifica colisiones después de mover
    QLabel *goku = ui->gokuLabel;
    int newX = goku->x() + dx;
    int newY = goku->y() + dy;

    // Limita el movimiento dentro de los bordes del widget
    newX = qMax(0, qMin(newX, width() - goku->width()));
    newY = qMax(0, qMin(newY, height() - goku->height()));

    goku->move(newX, newY);
    verificarColisiones();  // Verifica colisiones después de mover
    QRect zonaMeta(width() - 100, 50, 80, 100);  // Ajusta según la meta
    if (zonaMeta.intersects(ui->gokuLabel->geometry())) {
        nivelCompletado();
    }

}
void segundo::keyPressEvent(QKeyEvent *event) {
    int paso = 10;  // Píxeles a mover
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        moverGoku(0, -paso);  // Arriba
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        moverGoku(0, paso);   // Abajo
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        moverGoku(-paso, 0); // Izquierda
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        moverGoku(paso, 0);  // Derecha
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}


void segundo::reducirVida(int cantidad) {
    vidaActual = qMax(0, vidaActual - cantidad);
    barraVida->setValue(vidaActual);

    // Efecto de parpadeo al recibir daño
    QGraphicsOpacityEffect *efecto = new QGraphicsOpacityEffect(ui->gokuLabel);
    ui->gokuLabel->setGraphicsEffect(efecto);
    QPropertyAnimation *animacion = new QPropertyAnimation(efecto, "opacity");
    animacion->setDuration(500);  // 0.5 segundos
    animacion->setStartValue(1);  // Visible
    animacion->setEndValue(0.2);  // Casi transparente
    animacion->setEasingCurve(QEasingCurve::OutQuad);
    animacion->start(QAbstractAnimation::DeleteWhenStopped);

    if (vidaActual <= 0) {
        QMessageBox::critical(this, "Game Over", "¡Goku ha sido derrotado!");
        close();  // Cierra la ventana
    }
}

void segundo::on_btnContinuar_clicked()
{
    if (!juego) {
        juego = new tercero(nullptr);  // Sin padre
    }
    juego->show();
    this->hide();  // Opcional: ocultar Primero
    juego = new tercero(this);
    this->hide();  // Oculta Primero
    juego->show();
}

void segundo::nivelCompletado()
{
    QMessageBox::information(this, "¡Nivel completado!", "Has llegado al final.");
    ui->btnContinuar->show();  // ✅ Mostrar el botón ahora
}
