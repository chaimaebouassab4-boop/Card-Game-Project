#include "statt.h"
#include "mainwindow.h"
#include <QPainter>

Statt::Statt(QVector2D pos, QVector2D size, QObject *parent)
    : QObject{parent}
{
    this->pos = pos;
    this->size = size;
    this->parent = parent;
}

void Statt::draw() {
    MainWindow *main = qobject_cast<MainWindow*>(parent);
    QPainter painter(main);

//    QString message =
//        main->winner == 1 ?
//            "🎉 Bravo, vous avez gagné(e)." :
//        main->winner == 0 ? "🤝 Équivalent." :
//        "😕 Vous avez perdu.";

    QString message;

    switch (main->winner) {
    case 1:
        message = " 🎉Bravo, vous avez gagné.";
            break;
    case 0:
        message = " 🤝 Équivalent.";
            break;
    case -1:
        message = " 😕 Vous avez perdu !";
        break;
    }

    painter.setOpacity(0.8);

    painter.setOpacity(0.8);
    painter.fillRect(0, 0, main->width() * scaleX, main->height(), QBrush(QColor(0, 0, 0, 200)));
    painter.setOpacity(1);
    if (main->winner == 1 && main->resultElapsed.isValid() && main->resultElapsed.elapsed() < 3000) {
        const qreal progress = main->resultElapsed.elapsed() / 3000.0;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setOpacity(qMin(1.0, (1.0 - progress) * 3));
        const QColor colors[] = {QColor("#ffcf66"), QColor("#f674b5"), QColor("#bca1ff")};
        painter.setPen(Qt::NoPen);
        for (int i = 0; i < 36; ++i) {
            painter.save();
            painter.translate((i * 137 + 43) % main->width(),
                              -100 + progress * (main->height() + 240) - (i % 5) * 35);
            painter.rotate(i * 23 + progress * 240);
            painter.setBrush(colors[i % 3]);
            painter.drawRoundedRect(QRectF(-3, -6, 6, 12), 1, 1);
            painter.restore();
        }
        painter.setOpacity(1);
    }

    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setFont(QFont("Times", 18, 600));
    painter.drawText(0, 0, main->width() * scaleX, main->height(), Qt::AlignCenter, message);
}
