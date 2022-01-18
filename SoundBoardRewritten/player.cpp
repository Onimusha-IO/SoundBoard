#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QWidget(parent),
    ui(new Ui::Player){
    ui->setupUi(this);

     ui->pushButton->setCheckable(true);
     ui->horizontalSlider->setValue(50);

     mPlayer = new QMediaPlayer(this);
     mPlaylist = new QMediaPlaylist(this);
     mPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
     mPlayer->setPlaylist(mPlaylist);
     mPlayer->setVolume(ui->horizontalSlider->value());

     connect(ui->pushButton, &QPushButton::clicked, this, &Player::playStopButton);
     connect(ui->pushButton_2, &QPushButton::clicked, this, &Player::emitThisPlayer);
     connect(mPlayer, &QMediaPlayer::stateChanged, this, &Player::soundEnd);
     connect(ui->checkBox, &QCheckBox::clicked, this, &Player::loop);
     connect(ui->horizontalSlider, &QSlider::valueChanged, mPlayer, &QMediaPlayer::setVolume);

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
     ui->pushButton_2->setIcon(QIcon(QPixmap(":/icons/minus.png")));
     ui->pushButton_2->setIconSize(QSize(28, 28));
}

Player::~Player(){
    delete ui;
}

QPushButton* Player::getPlay(){
    return ui->pushButton;
}

QCheckBox* Player::getLoop(){
    return ui->checkBox;
}

void Player::emitThisPlayer(bool checked){
    (void)checked;
    emit thisPlayer(this);
}

QMediaPlayer *Player::getPlayer(){
    return mPlayer;
}

QMediaPlaylist *Player::getPlaylist(){
    return mPlaylist;
}

void Player::setTitle(QString title){
    ui->label->setText(title);
}

void Player::stop(){
    mPlayer->stop();
}

void Player::loop(bool checked){
    if(checked){
        mPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }else{
        mPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    }
}

void Player::soundEnd(QMediaPlayer::State state){
    if(state == QMediaPlayer::State::StoppedState){        
        playStopButton(false);
        ui->pushButton->setChecked(false);
    }
}

void Player::playStopButton(bool checked){
    if(checked){
        ui->pushButton->setStyleSheet("background-color: transparent;");
        ui->pushButton->setIcon(QIcon(QPixmap(":/icons/stop.png")));
        ui->pushButton->setIconSize(QSize(28, 28));
        mPlayer->play();
    } else{
        ui->pushButton->setStyleSheet("background-color: transparent;");
        ui->pushButton->setIcon(QIcon(QPixmap(":/icons/play.png")));
        ui->pushButton->setIconSize(QSize(28, 28));
        mPlayer->stop();
    }
}
