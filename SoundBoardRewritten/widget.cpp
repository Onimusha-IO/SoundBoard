#include <QFileDialog>
#include <QInputDialog>

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent),
    ui(new Ui::Widget){
    ui->setupUi(this);

    setSoundLayout();
    setSceneLayout();

    addTrackButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addTrackButton->setStyleSheet("background-color: transparent;");
    addTrackButton->setIcon(QIcon(QPixmap(":icons/add.png")));
    addTrackButton->setIconSize(QSize(38, 38));
    addTrackButton->setToolTip("Add Mp3 Track");
    addTrackButton->setToolTipDuration(1000);

    stopAllTracksButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stopAllTracksButton->setStyleSheet("background-color: transparent;");
    stopAllTracksButton->setIcon(QIcon(QPixmap(":icons/stopAll.png")));
    stopAllTracksButton->setIconSize(QSize(38, 38));
    stopAllTracksButton->setToolTip("Stop All Tracks");
    stopAllTracksButton->setToolTipDuration(1000);

    removeAllTracksButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    removeAllTracksButton->setStyleSheet("background-color: transparent;");
    removeAllTracksButton->setIcon(QIcon(QPixmap(":icons/closeAll.png")));
    removeAllTracksButton->setIconSize(QSize(38, 38));
    removeAllTracksButton->setToolTip("Remove All Tracks");
    removeAllTracksButton->setToolTipDuration(1000);

    addSceneButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addSceneButton->setStyleSheet("background-color: transparent;");
    addSceneButton->setIcon(QIcon(QPixmap(":icons/scene.png")));
    addSceneButton->setIconSize(QSize(38, 38));
    addSceneButton->setToolTip("Add Mp3 Track");
    addSceneButton->setToolTipDuration(1000);

    stopAllScenesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stopAllScenesButton->setStyleSheet("background-color: transparent;");
    stopAllScenesButton->setIcon(QIcon(QPixmap(":icons/stopAll.png")));
    stopAllScenesButton->setIconSize(QSize(38, 38));
    stopAllScenesButton->setToolTip("Stop All Tracks");
    stopAllScenesButton->setToolTipDuration(1000);

    removeAllScenesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    removeAllScenesButton->setStyleSheet("background-color: transparent;");
    removeAllScenesButton->setIcon(QIcon(QPixmap(":icons/closeAll.png")));
    removeAllScenesButton->setIconSize(QSize(38, 38));
    removeAllScenesButton->setToolTip("Remove All Tracks");
    removeAllScenesButton->setToolTipDuration(1000);

    connect(addTrackButton, &QPushButton::clicked, this, &Widget::addTrack);
    connect(stopAllTracksButton, &QPushButton::clicked, this, &Widget::stopAllTracks);
    connect(removeAllTracksButton, &QPushButton::clicked, this, &Widget::removeAllTracks);

    connect(addSceneButton, &QPushButton::clicked, this, &Widget::addScene);
    connect(stopAllScenesButton, &QPushButton::clicked, this, &Widget::stopAllScenes);
    connect(removeAllScenesButton, &QPushButton::clicked, this, &Widget::removeAllScenes);

    this->resize(800, 600);
    this->setWindowTitle("TableTop SoundBoard v1.5");
}

void Widget::removeSound(Player *player){
    players.removeAll(player);
    player->~Player();

    soundGrid->~QGridLayout();
    soundGrid = new QGridLayout;
    hSoundLayout->insertLayout(hSoundLayout->count() - 1, soundGrid);

    soundRow = 0;
    soundCol = 0;
    int counter = 0;
    while(counter < players.size()){
        if(soundCol < 5){
            soundGrid->addWidget(players.at(counter), soundRow, soundCol);
            ++soundCol;
        } else{
            soundCol = 0;
            ++soundRow;
            soundGrid->addWidget(players.at(counter), soundRow, soundCol);
            ++soundCol;
        }
        ++counter;
    }
}

void Widget::setSoundLayout(){
    soundCol = 0;
    soundRow = 0;

    addTrackButton = new QPushButton;
    removeAllTracksButton = new QPushButton;
    stopAllTracksButton = new QPushButton;

    soundGrid = new QGridLayout;
    soundPanel = new QWidget;
    soundPanelLayout = new QHBoxLayout;
    hSoundLayout = new QHBoxLayout;
    vSoundLayout = new QVBoxLayout(ui->tabWidget->widget(0));

    soundPanelLayout->addWidget(addTrackButton);
    soundPanelLayout->addWidget(removeAllTracksButton);
    soundPanelLayout->addWidget(stopAllTracksButton);
    soundPanelLayout->addStretch(1);

    soundPanel->setLayout(soundPanelLayout);

    hSoundLayout->addLayout(soundGrid);
    hSoundLayout->addStretch(1);

    vSoundLayout->addWidget(soundPanel);
    vSoundLayout->addLayout(hSoundLayout);
    vSoundLayout->addStretch(1);
}

void Widget::setSceneLayout(){
    sceneCol = 0;
    sceneRow = 0;

    addSceneButton = new QPushButton;
    removeAllScenesButton = new QPushButton;
    stopAllScenesButton = new QPushButton;

    sceneGrid = new QGridLayout;
    scenePanel = new QWidget;
    scenePanelLayout = new QHBoxLayout;
    hSceneLayout = new QHBoxLayout;
    vSceneLayout = new QVBoxLayout(ui->tabWidget->widget(1));

    scenePanelLayout->addWidget(addSceneButton);
    scenePanelLayout->addWidget(removeAllScenesButton);
    scenePanelLayout->addWidget(stopAllScenesButton);
    scenePanelLayout->addStretch(1);

    scenePanel->setLayout(scenePanelLayout);

    hSceneLayout->addLayout(sceneGrid);
    hSceneLayout->addStretch(1);

    vSceneLayout->addWidget(scenePanel);
    vSceneLayout->addLayout(hSceneLayout);
    vSceneLayout->addStretch(1);
}

void Widget::addTrack(bool checked){
    (void)checked;

    QFileDialog fileDialog;
    QStringList fileList = fileDialog.getOpenFileNames
            (this, "Open MP3", QDir::currentPath(), "MP3 files (*.mp3)");

    if(!fileList.isEmpty()){
        for(int i = 0; i < fileList.size(); ++i){
            if(soundCol < 5){
                players.append(new Player(this));
                connect(players.at(players.size() - 1), &Player::thisPlayer, this, &Widget::removeSound);
                qDebug() << players.size() << endl;
                players.at(players.size() - 1)->getPlaylist()->addMedia(QUrl(fileList.at(i)));
                QString file = fileList.at(i);
                file.remove(0, file.lastIndexOf('/') + 1);
                file.chop(4);
                players.at(players.size() - 1)->setTitle(file);
                soundGrid->addWidget(players.at(players.size() - 1), soundRow, soundCol);
                ++soundCol;
            } else{
                soundCol = 0;
                soundRow++;
                players.append(new Player(this));
                connect(players.at(players.size() - 1), &Player::thisPlayer, this, &Widget::removeSound);
                qDebug() << players.size() << endl;
                players.at(players.size() - 1)->getPlaylist()->addMedia(QUrl(fileList.at(i)));
                QString file = fileList.at(i);
                file.remove(0, file.lastIndexOf('/') + 1);
                file.chop(4);
                players.at(players.size() - 1)->setTitle(file);
                soundGrid->addWidget(players.at(players.size() - 1), soundRow, soundCol);
                ++soundCol;
            }
        }
    } else{
        return;
    }

}

void Widget::removeAllTracks(bool checked){
    (void)checked;
    for(int i = 0; i < players.size(); ++i){
        players.at(i)->~Player();
    }
    players.clear();
    soundCol = 0;
    soundRow = 0;
}

void Widget::stopAllTracks(bool checked){
    (void)checked;
    for(int i = 0; i < players.size(); ++i){
        players.at(i)->stop();
    }
}

void Widget::addScene(bool checked){
    (void)checked;

    bool ok;
    QString title = QInputDialog::getText(this, "Scene Title",
                                               "Title: ", QLineEdit::Normal,
                                               "", &ok);
    if (ok && !title.isEmpty()){
        if(sceneCol < 5){
            Scene *scene = new Scene;
            connect(scene, &Scene::removed, this, &Widget::removeScene);
            scenes.append(scene);
            scene->setTitle(title);
            sceneGrid->addWidget(scene, sceneRow, sceneCol);
            ++sceneCol;
        } else{
            sceneCol = 0;
            ++sceneRow;
            Scene *scene = new Scene;
            connect(scene, &Scene::removed, this, &Widget::removeScene);
            scenes.append(scene);
            scene->setTitle(title);
            sceneGrid->addWidget(scene, sceneRow, sceneCol);
            ++sceneCol;
        }
    } else{
        return;
    }
}

void Widget::removeScene(Scene *scene){
    scenes.removeAll(scene);
    scene->~Scene();

    sceneGrid->~QGridLayout();
    sceneGrid = new QGridLayout;
    hSceneLayout->insertLayout(hSceneLayout->count() - 1, sceneGrid);

    sceneRow = 0;
    sceneCol = 0;
    int counter = 0;
    while(counter < scenes.size()){
        if(sceneCol < 5){
            sceneGrid->addWidget(scenes.at(counter), sceneRow, sceneCol);
            ++sceneCol;
        } else{
            sceneCol = 0;
            ++sceneRow;
            sceneGrid->addWidget(scenes.at(counter), sceneRow, sceneCol);
            ++sceneCol;
        }
        ++counter;
    }
}

void Widget::stopAllScenes(bool checked){
    (void)checked;

}

void Widget::removeAllScenes(bool checked){
    (void)checked;

    for(int i = 0; i < scenes.size(); ++i){
        scenes.at(i)->~Scene();
    }
    scenes.clear();
    sceneCol = 0;
    sceneRow = 0;
}

Widget::~Widget(){
    delete ui;
}
