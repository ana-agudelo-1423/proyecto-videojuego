#include "tercero.h"
#include "ui_tercero.h"
#include <QApplication>
#include <QPixmap>
#include <QRandomGenerator>
#include <QTimer>
#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QProgressBar>
#include <QHBoxLayout>

tercero::tercero(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tercero)
{
    ui->setupUi(this); // Solo una vez

        // Carga segura de imagen
        if (!fondotercero.load(
                "C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/"
                "Desktop_Qt_6_8_2_MinGW_64_bit-Debug/fondotercero.png")) {
            qWarning() << "No se pudo cargar la imagen. Usando color sólido";
            fondotercero = QPixmap(size());
            fondotercero.fill(Qt::darkBlue);
        }

        // Configurar fondo solo una vez en el constructor
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondotercero));
        this->setPalette(palette);
        this->setAutoFillBackground(true);



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
        ui->gokuLabel->setPixmap(gokuImg.scaled(111, 131, Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
    } else {
        ui->gokuLabel->setText("Goku aquí (imagen no encontrada)");
    }
    jugador = new Personaje("Goku", 100, 20, 10);
    enemigo = new Personaje("Piccolo", 80, 18, 8);


    // Configurar timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &tercero::actualizarUI);

    // Configurar botones
    connect(ui->btnIniciar, &QPushButton::clicked, this, &tercero::iniciarPelea);
   connect(ui->btnAtaque, &QPushButton::clicked, this, &tercero::on_btnAtaque_clicked);

    // Estado inicial
    actualizarUI();
   timerBolas = new QTimer(this);
   connect(timerBolas, &QTimer::timeout, this, &tercero::moverBolas);
   timerBolas->start(30);
    ui->btnAtaque->setEnabled(false);
   QProgressBar *barraVida = new QProgressBar;
   barraVida->setRange(0, 100);
   barraVida->setValue(100); // Vida inicial
   barraVida->setTextVisible(true);
   barraVida->setStyleSheet(
       "QProgressBar {"
       " border: 2px solid black;"
       " border-radius: 5px;"
       " text-align: center;"
       " background: white;"
       "}"
       "QProgressBar::chunk {"
       " background-color: red;"
       " width: 10px;"
       "}"
       );

   // Texto central
   QLabel *textoPelea = new QLabel("");
   textoPelea->setStyleSheet("font-size: 24px; color: white;");
   textoPelea->setAlignment(Qt::AlignCenter);

   // Botón de ataque
   QPushButton *botonAtaque = new QPushButton("¡ATACAR!");
   botonAtaque->setStyleSheet("font-size: 20px; background-color: red; color: white; border-radius: 10px;");
   QObject::connect(botonAtaque, &QPushButton::clicked, [=]() {
       int vidaActual = barraVida->value();
       if (vidaActual > 0) {
           barraVida->setValue(vidaActual - 10); // Quitar 10 de vida
           textoPelea->setText("¡Kamehameha!");
       } else {
           textoPelea->setText("¡Victoria de Goku!");
       }
   });

   vidaGoku = new QProgressBar(this);
   vidaGoku->setRange(0, 100);
   vidaGoku->setValue(100);

   vidaPiccolo = new QProgressBar(this);
   vidaPiccolo->setRange(0, 100);
   vidaPiccolo->setValue(100);
};
void tercero::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // Solo una llamada

    // Si necesitas escalar el fondo al redimensionar, puedes hacerlo aquí
    if (!fondotercero.isNull()) {
        QPixmap fondoEscalado = fondotercero.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(fondoEscalado));
        this->setPalette(palette);
    }
}
tercero::~tercero()
{
    delete jugador;
    delete enemigo;
    delete ui;
}
void tercero::iniciarPelea()
{
    ui->btnIniciar->setEnabled(false);
    ui->btnAtaque->setEnabled(true);
    turnoActual = true; // Empieza el jugador
    actualizarUI();
}

void tercero::turnoJugador()
{
    if (jugador->atacar(*enemigo)) {
        QMessageBox::information(this, "Fin de Partida", "¡Has derrotado al enemigo!");
        resetearJuego();
        return;
    }

    turnoActual = false;
    actualizarUI();

    // El enemigo ataca después de un breve retraso
    QTimer::singleShot(1500, this, &tercero::turnoEnemigo);
}

void tercero::turnoEnemigo()
{
    if (enemigo->atacar(*jugador)) {
        QMessageBox::information(this, "Fin de Partida", "¡Has sido derrotado!");
        resetearJuego();
        return;
    }

    turnoActual = true;
    actualizarUI();
}

void tercero::actualizarUI()
{
    // Imagen de Goku
    QString rutaGoku = "C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/goku.png";
    QFile fileGoku(rutaGoku);
    if (fileGoku.exists()) {
        QPixmap gokuImg(rutaGoku);
        if (!gokuImg.isNull()) {
            ui->gokuLabel->setPixmap(gokuImg.scaled(111, 131, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QPixmap vacia(111, 131); vacia.fill(Qt::gray);
            ui->gokuLabel->setPixmap(vacia);
            qWarning() << "⚠️ goku.png está dañado";
        }
    } else {
        QPixmap vacia(111, 131); vacia.fill(Qt::red);
        ui->gokuLabel->setPixmap(vacia);
        qWarning() << "❌ goku.png no encontrado en: " << rutaGoku;
    }

    // Info de Goku
    ui->gokuInfoLabel->setText(QString("%1\nVida: %2\nAtaque: %3\nDefensa: %4")
                                   .arg(jugador->getNombre())
                                   .arg(jugador->getVida())
                                   .arg(jugador->getAtaque())
                                   .arg(jugador->getDefensa()));

    // Imagen de Piccolo
    QString rutaPiccolo = "C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/piccolo.png";
    QFile filePiccolo(rutaPiccolo);
    if (filePiccolo.exists()) {
        QPixmap piccoloImg(rutaPiccolo);
        if (!piccoloImg.isNull()) {
            ui->piccoloLabel->setPixmap(piccoloImg.scaled(111, 131, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QPixmap vacia(111, 131); vacia.fill(Qt::gray);
            ui->piccoloLabel->setPixmap(vacia);
            qWarning() << "⚠️ piccolo.png está dañado";
        }
    } else {
        QPixmap vacia(111, 131); vacia.fill(Qt::red);
        ui->piccoloLabel->setPixmap(vacia);
        qWarning() << "❌ piccolo.png no encontrado en: " << rutaPiccolo;
    }

    // Info de Piccolo
    ui->piccoloInfoLabel->setText(QString("%1\nVida: %2\nAtaque: %3\nDefensa: %4")
                                      .arg(enemigo->getNombre())
                                      .arg(enemigo->getVida())
                                      .arg(enemigo->getAtaque())
                                      .arg(enemigo->getDefensa()));

    // Habilitar ataque solo si es turno del jugador
    ui->btnAtaque->setEnabled(turnoActual);
}
void tercero::resetearJuego()
{
    delete jugador;
    delete enemigo;

    jugador = new Personaje("Goku", 100, 20, 10);
    enemigo = new Personaje("Piccolo", 80, 18, 8);

    ui->btnIniciar->setEnabled(true);
    ui->btnAtaque->setEnabled(false);
    actualizarUI();
}

void tercero::on_btnSalir_clicked()
{
    auto respuesta = QMessageBox::question(this, "Salir", "¿Seguro que quieres salir?");
    if (respuesta == QMessageBox::Yes) {
        this->close();
    }
}
void tercero::lanzarBolaDeEnergia()
{
    QLabel* bola = new QLabel(this);
    bola->setPixmap(QPixmap("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/bolaGoku.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bola->setGeometry(ui->gokuLabel->x() + ui->gokuLabel->width(), ui->gokuLabel->y() + 50, 32, 32);
    bola->show();
    bolasEnergia.append(bola);
}

void tercero::moverBolas()
{
    for (int i = bolasEnergia.size() - 1; i >= 0; --i) {
        QLabel* bola = bolasEnergia[i];
        bool esDelEnemigo = bola->property("enemigo").toBool();

        if (esDelEnemigo) {
            // Mover hacia la izquierda
            bola->move(bola->x() - 10, bola->y());

            // Colisión con Goku
            if (bola->geometry().intersects(ui->gokuLabel->geometry())) {
                jugador->recibirDanio(enemigo->getAtaque());  // Aplica daño real
                qDebug() << "⚠️ Goku recibió daño";
                bola->hide();
                delete bola;
                bolasEnergia.removeAt(i);
                actualizarUI();
                continue;
            }

            // Sale por la izquierda
            if (bola->x() + bola->width() < 0) {
                bola->hide();
                delete bola;
                bolasEnergia.removeAt(i);
                continue;
            }

        } else {
            // Mover hacia la derecha
            bola->move(bola->x() + 10, bola->y());

            // Colisión con Piccolo
            if (bola->geometry().intersects(ui->piccoloLabel->geometry())) {
                enemigo->recibirDanio(jugador->getAtaque());  // Aplica daño real
                qDebug() << "💥 Piccolo recibió daño";
                bola->hide();
                delete bola;
                bolasEnergia.removeAt(i);
                actualizarUI();

                // Piccolo contraataca después de 1 segundo
                QTimer::singleShot(1000, this, &tercero::lanzarBolaDeEnergiaEnemigo);
                continue;
            }

            // Sale por la derecha
            if (bola->x() > width()) {
                bola->hide();
                delete bola;
                bolasEnergia.removeAt(i);
                continue;
            }
        }
    }
}


void tercero::on_btnAtaque_clicked()
{
    int vida = vidaPiccolo->value();
lanzarBolaDeEnergia();
    if (vida > 0) {
        vidaPiccolo->setValue(vida - 10);
    }

    if (vida <= 10) { // ya llegó a 0 o menos con el ataque actual
        QMessageBox msgBox;
        msgBox.setWindowTitle("¡Victoria!");
        msgBox.setText("¡Goku ha ganado la batalla!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }

}
void tercero::lanzarBolaDeEnergiaEnemigo()
{
    QLabel* bola = new QLabel(this);
    bola->setPixmap(QPixmap("C:/Users/Usuario/Documents/proyecto-videojuego/videojuego/build/Desktop_Qt_6_8_2_MinGW_64_bit-Debug/bolaPiccolo.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bola->setGeometry(ui->piccoloLabel->x() - 32, ui->piccoloLabel->y() + 50, 32, 32);
    bola->show();
    bolasEnergia.append(bola);
    bola->setProperty("enemigo", true);  // Marcar que esta bola viene de Piccolo
}

void Personaje::recibirDanio(int danio)
{
    int danioFinal = danio - defensa;
    if (danioFinal < 1) danioFinal = 1;  // Siempre al menos 1 daño
    vida -= danioFinal;
    if (vida < 0) vida = 0;
}
