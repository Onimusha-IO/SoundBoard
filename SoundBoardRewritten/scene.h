#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "player.h"

namespace Ui {
    class Scene;
}

class  Editor : public QWidget{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
     ~Editor();

    void resizeEvent(QResizeEvent *evt);

};

class Scene : public QWidget{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = 0);
    ~Scene();

    void setTitle(QString);
    void setLayout();

public slots:
    void emitThisScene(bool);
    void showEditor(bool);
    void playStop(bool);
    void loopScene(bool);
    void playerStateChanged(QMediaPlayer::State);
    void soundEnd();

    void addSound(bool);
    void removeSound(Player *player);
    void removeAllSounds(bool);

signals:
    void removed(Scene*);

private:
    Ui::Scene *ui;
    QVector<Player*> players;

    Editor *editor;

    QPushButton *add;
    QPushButton *removeAll;

    QGridLayout *grid;
    QWidget *panel;

    QHBoxLayout *h;
    QVBoxLayout *v;
    QHBoxLayout *panelLayout;

    int col;
    int row;

    int stoppedCounter;
};

#endif // SCENE_H
