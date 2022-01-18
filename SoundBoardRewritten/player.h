#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCheckBox>
#include<QPushButton>

namespace Ui {
    class Player;
}

class Player : public QWidget{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

    void setTitle(QString);
    QMediaPlaylist *getPlaylist();
    void stop();
    QMediaPlayer *getPlayer();
    QCheckBox* getLoop();
    QPushButton* getPlay();

public slots:
    void playStopButton(bool);
    void soundEnd(QMediaPlayer::State);
    void loop(bool);
    void emitThisPlayer(bool);

signals:
    void thisPlayer(Player*);
    void stateChanged(QMediaPlayer::State);

private:
    Ui::Player *ui;

    QMediaPlayer *mPlayer;
    QMediaPlaylist *mPlaylist;
};

#endif // PLAYER_H
