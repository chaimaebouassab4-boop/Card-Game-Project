#include "menu.h"
#include "mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QStackedWidget>

Menu::Menu(QObject *parent) : QObject(parent)
{
    auto *main = qobject_cast<MainWindow *>(parent);
    main->setWindowTitle(tr("Ronda"));
    auto *pages = new QStackedWidget(main);
    pages->setGeometry(main->rect());
    auto *home = new QWidget;
    pages->addWidget(home);
    auto *art = new QLabel(home);
    art->setFixedSize(main->size());
    art->setAlignment(Qt::AlignCenter);
    const QPixmap cover(":/recent/textures-ch/ronda-cover.png");
    const qreal dpr = main->devicePixelRatioF();
    QPixmap artwork = cover.scaled(main->size() * dpr, Qt::KeepAspectRatioByExpanding,
                                   Qt::SmoothTransformation);
    artwork.setDevicePixelRatio(dpr);
    art->setPixmap(artwork);
    art->setAccessibleName(tr("Ronda: The classic Moroccan card game"));

    // Keep the editor and actions as real keyboard-accessible Qt controls.
    auto *field = new QFrame(home);
    field->setObjectName("playerField");
    field->setGeometry(253, 344, 360, 62);
    field->setStyleSheet("#playerField{background:#30143f;border:1px solid #cf8dc4;border-radius:14px;}");
    auto *avatar = new QLabel(field);
    avatar->setGeometry(12, 11, 40, 40);
    avatar->setPixmap(QPixmap(":/recent/textures-ch/menu-profile.png"));
    avatar->setScaledContents(true);
    auto *caption = new QLabel(tr("USERNAME"), field);
    caption->setGeometry(64, 5, 280, 21);
    caption->setStyleSheet("color:#efdbec;font:600 12px 'Segoe UI';letter-spacing:1px;");
    auto *name = new QLineEdit(field);
    caption->setBuddy(name);
    name->setAccessibleName(tr("Username"));
    name->setGeometry(61, 25, 282, 30);
    name->setMaxLength(30);
    name->setPlaceholderText(tr("Enter your name"));
    name->setStyleSheet("QLineEdit{background:transparent;color:white;border:0;border-bottom:1px solid transparent;"
                        "font:17px 'Segoe UI';placeholder-text-color:#c3a8ca;}"
                        "QLineEdit:focus{border-bottom-color:#916292;}");

    auto *start = new QPushButton(tr("♠    START GAME    ›"), home);
    start->setAccessibleName(tr("Start game"));
    start->setGeometry(253, 419, 360, 56);
    start->setCursor(Qt::PointingHandCursor);
    start->setStyleSheet("QPushButton{color:#fff8f3;border:2px solid #ffd980;border-radius:15px;"
                         "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f03c94,stop:1 #ae0e60);"
                         "font:700 24px 'Segoe UI';letter-spacing:1px;}"
                         "QPushButton:hover{background:#e3338b;} QPushButton:focus{border-color:white;}"
                         "QPushButton:pressed{background:#a31559;}");
    auto *glow = new QGraphicsDropShadowEffect(start);
    glow->setBlurRadius(18);
    glow->setOffset(0, 0);
    glow->setColor(QColor("#ef9755"));
    start->setGraphicsEffect(glow);

    const QString linkStyle = "QPushButton{color:#fff5fa;background:#281237;border:1px solid #af70a8;"
                              "border-radius:10px;font:600 15px 'Segoe UI';letter-spacing:1px;}"
                              "QPushButton:hover{background:#49204f;} QPushButton:focus{border-color:#ffcf80;}";
    auto *rules = new QPushButton(tr("HOW TO PLAY"), home);
    rules->setGeometry(298, 487, 270, 43);
    rules->setCursor(Qt::PointingHandCursor);
    rules->setStyleSheet(linkStyle);
    rules->setIcon(QIcon(":/recent/textures-ch/menu-book.png"));
    rules->setIconSize(QSize(31, 23));

    auto *robot = new QLabel(home);
    robot->setGeometry(414, 544, 38, 37);
    robot->setPixmap(QPixmap(":/recent/textures-ch/menu-computer.png"));
    robot->setScaledContents(true);
    auto *mode = new QLabel(tr("VS COMPUTER"), home);
    mode->setGeometry(253, 583, 360, 25);
    mode->setAlignment(Qt::AlignCenter);
    mode->setStyleSheet("color:#ffe1a1;font:700 16px 'Segoe UI';letter-spacing:2px;");
    auto *opponent = new QLabel(tr("Single player  ·  AI opponent"), home);
    opponent->setGeometry(253, 609, 360, 23);
    opponent->setAlignment(Qt::AlignCenter);
    opponent->setStyleSheet("color:#e3cee9;font:14px 'Segoe UI';");

    auto *help = new QWidget;
    pages->addWidget(help);
    help->setStyleSheet("background:#230f35;");
    auto *instructions = new QLabel(help);
    instructions->setGeometry(26, 78, main->width() - 52, main->height() - 170);
    instructions->setAlignment(Qt::AlignCenter);
    instructions->setPixmap(QPixmap(":/regles/rules.png").scaled(instructions->size(),
                            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    auto *lastCaptureRule = new QLabel(
        tr("End of the game: whoever last captured cards collects all cards left on the table. "
           "Each remaining card adds one point to their final score."), help);
    lastCaptureRule->setGeometry(36, main->height() - 82, main->width() - 72, 64);
    lastCaptureRule->setWordWrap(true);
    lastCaptureRule->setStyleSheet("color:#ffe1a1;font:15px 'Segoe UI';");
    auto *back = new QPushButton(tr("Back to menu"), help);
    back->setGeometry(24, 20, 126, 40);
    back->setCursor(Qt::PointingHandCursor);
    back->setStyleSheet(linkStyle);
    connect(rules, &QPushButton::clicked, pages, [=] {
        pages->setCurrentWidget(help);
        back->setFocus();
    });
    connect(back, &QPushButton::clicked, pages, [=] {
        pages->setCurrentWidget(home);
        rules->setFocus();
    });
    connect(name, &QLineEdit::returnPressed, start, &QPushButton::click);
    connect(name, &QLineEdit::textEdited, caption, [caption] { caption->setText(tr("USERNAME")); });
    connect(start, &QPushButton::clicked, pages, [=] {
        if (name->text().trimmed().isEmpty()) {
            caption->setText(tr("Enter a name to start"));
            name->setFocus();
            return;
        }
        main->playerName = name->text().trimmed();
        main->gameStarted = true;
        pages->hide();
        pages->deleteLater();
    });
    pages->raise();
    name->setFocus();
}
