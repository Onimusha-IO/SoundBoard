#include "scene.h"
#include "ui_scene.h"

#include <QFileDialog>

Scene::Scene(QWidget *parent) : QWidget(parent),
    ui(new Ui::Scene){
    ui->setupUi(this);

    stoppedCounter = 0;

    setLayout();

    add->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    add->setStyleSheet("background-color: transparent;");
    add->setIcon(QIcon(QPixmap(":icons/add.png")));
    add->setIconSize(QSize(38, 38));
    add->setToolTip("Add Mp3 Track");
    add->setToolTipDuration(1000);

    removeAll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    removeAll->setStyleSheet("background-color: transparent;");
    removeAll->setIcon(QIcon(QPixmap(":icons/closeAll.png")));
    removeAll->setIconSize(QSize(38, 38));
    removeAll->setToolTip("Remove All Tracks");
    removeAll->setToolTipDuration(1000);

    ui->pushButton->setCheckable(true);
    connect(ui->pushButton, &QPushButton::clicked, this, &Scene::playStop);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Scene::showEditor);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Scene::emitThisScene);
    connect(ui->checkBox, &QCheckBox::clicked, this, &Scene::loopScene);

    ui->horizontalSlider->setDisabled(true);

    connect(add, &QPushButton::clicked, this, &Scene::addSound);
    connect(removeAll, &QPushButton::clicked, this, &Scene::removeAllSounds);

    QFont font;
    font.setBold(true);
    font.setFamily("Arial");
    font.setPointSize(10);
    ui->label->setFont(font);

    QColor color = QColor(Qt::white);
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->label->setPalette(palette);

    ui->pushButton->setStyleSheet("background-color: transparent;");
    ui->pushButton->setIcon(QIcon(QPixmap(":/icons/play.png")));
    ui->pushButton->setIconSize(QSize(28, 28));

    ui->pushButton_2->setStyleSheet("background-color: transparent;");
    ui->pushButton_2->setIcon(QIcon(QPixmap(":/icons/settings.png")));
    ui->pushButton_2->setIconSize(QSize(28, 28));

    ui->pushButton_3->setStyleSheet("background-color: transparent;");
    ui->pushButton_3->setIcon(QIcon(QPixmap(":/icons/minus.png")));
    ui->pushButton_3->setIconSize(QSize(28, 28));
}

Editor::Editor(QWidget *parent) : QWidget(parent){}

void Editor::resizeEvent(QResizeEvent *evt){
    QPixmap bkgnd(":/images/Scene.jpg");
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); //copy current, not create new
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QWidget::resizeEvent(evt); //call base implementation
}

Editor::~Editor(){

}

void Scene::soundEnd(){
    qDebug() << "soundEnd() Emitido" << endl;
    if(stoppedCounter == 0){
        qDebug() << "Scene play/stop Button checked false" << endl;
        playStop(false);
        ui->pushButton->setChecked(false);
    }
}

void Scene::playerStateChanged(QMediaPlayer::State state){
    qDebug() << "stateChanged() Emited" << endl;
    if(state == QMediaPlayer::State::PlayingState){
        ++stoppedCounter;
        qDebug() << "Counter ++: " << stoppedCounter << endl;
    } else{
        --stoppedCounter;
        qDebug() << "Counter --: " << stoppedCounter << endl;
    }
    soundEnd();
}

void Scene::loopScene(bool checked){
    for(int i = 0; i < players.size(); ++i){
        players.at(i)->getLoop()->setChecked(checked);
        players.at(i)->loop(checked);
    }
}

void Scene::playStop(bool checked){
    if(checked){
        ui->pushButton->setStyleSheet("background-color: transparent;");
        ui->pushButton->setIcon(QIcon(QPixmap(":/icons/stop.png")));
        ui->pushButton->setIconSize(QSize(28, 28));

        /* Si hay alguien tocando lo para */
        for(int i = 0; i < players.size(); ++i){
            if(players.at(i)->getPlayer()->state() == QMediaPlayer::PlayingState){
                players.at(i)->playStopButton(false);
            }
        }

        /* Los toca todos */
        for(int i = 0; i < players.size(); ++i){
            players.at(i)->playStopButton(checked);
        }
    } else{
        ui->pushButton->setStyleSheet("background-color: transparent;");
        ui->pushButton->setIcon(QIcon(QPixmap(":/icons/play.png")));
        ui->pushButton->setIconSize(QSize(28, 28));

        /* Si hay alguien tocando lo para */
        for(int i = 0; i < players.size(); ++i){
            if(players.at(i)->getPlayer()->state() == QMediaPlayer::PlayingState){
                players.at(i)->playStopButton(false);
            }
        }
    }
}

void Scene::removeAllSounds(bool checked){
    (void)checked;

    for(int i = 0; i < players.size(); ++i){        
        players.at(i)->~Player();
    }
    players.clear();
    col = 0;
    row = 0;
    stoppedCounter = 0;
}

void Scene::removeSound(Player *player){
    players.removeAll(player);
    if(player->getPlayer()->PlayingState == QMediaPlayer::PlayingState){
        --stoppedCounter;
    }
    player->~Player();

    grid->~QGridLayout();
    grid = new QGridLayout;
    h->insertLayout(h->count() - 1, grid);

    row = 0;
    col = 0;
    int counter = 0;
    while(counter < players.size()){
        if(col < 3){
            grid->addWidget(players.at(counter), row, col);
            ++col;
        } else{
            col = 0;
            ++row;
            grid->addWidget(players.at(counter), row, col);
            ++col;
        }
        ++counter;
    }    
    soundEnd();
}

void Scene::addSound(bool checked){
    (void)checked;

    QFileDialog fileDialog;
    QStringList fileList = fileDialog.getOpenFileNames
            (this, "Open MP3", QDir::currentPath(), "MP3 files (*.mp3)");

    if(!fileList.isEmpty()){
        for(int i = 0; i < fileList.size(); ++i){
            if(col < 3){
                players.append(new Player(this));
                connect(players.at(players.size() - 1), &Player::thisPlayer, this, &Scene::removeSound);
                connect(players.at(players.size() - 1)->getPlayer(), &QMediaPlayer::stateChanged,
                        this, &Scene::playerStateChanged);
                qDebug() << players.size() << endl;
                players.at(players.size() - 1)->getPlaylist()->addMedia(QUrl(fileList.at(i)));
                QString file = fileList.at(i);
                file.remove(0, file.lastIndexOf('/') + 1);
                file.chop(4);
                players.at(players.size() - 1)->setTitle(file);
                grid->addWidget(players.at(players.size() - 1), row, col);
                ++col;
            } else{
                col = 0;
                row++;
                players.append(new Player(this));
                connect(players.at(players.size() - 1), &Player::thisPlayer, this, &Scene::removeSound);
                connect(players.at(players.size() - 1)->getPlayer(), &QMediaPlayer::stateChanged,
                        this, &Scene::playerStateChanged);
                qDebug() << players.size() << endl;
                players.at(players.size() - 1)->getPlaylist()->addMedia(QUrl(fileList.at(i)));
                QString file = fileList.at(i);
                file.remove(0, file.lastIndexOf('/') + 1);
                file.chop(4);
                players.at(players.size() - 1)->setTitle(file);
                grid->addWidget(players.at(players.size() - 1), row, col);
                ++col;
            }
        }
    } else{
        return;
    }

}

void Scene::setLayout(){
    col = 0;
    row = 0;

    editor = new Editor(this);
    editor->setWindowFlags(Qt::Window);
    panel = new QWidget;

    add = new QPushButton;
    removeAll = new QPushButton;

    grid = new QGridLayout;
    panelLayout = new QHBoxLayout;
    h = new QHBoxLayout;
    v = new QVBoxLayout(editor);

    panelLayout->addWidget(add);
    panelLayout->addWidget(removeAll);
    panelLayout->addStretch(1);

    panel->setLayout(panelLayout);

    h->addLayout(grid);
    h->addStretch(1);

    v->addWidget(panel);
    v->addLayout(h);
    v->addStretch(1);
}

void Scene::showEditor(bool checked){
    (void)checked;

    editor->setWindowTitle("Scene Editor: " + ui->label->text());
    editor->resize(400, 300);
    editor->show();
}

void Scene::emitThisScene(bool checked){
    (void)checked;
    emit removed(this);
}

void Scene::setTitle(QString title){
    ui->label->setText(title);
}

Scene::~Scene(){
    delete ui;
}
