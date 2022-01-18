#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>

#include "scene.h"
#include "player.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void setSoundLayout();
    void setSceneLayout();

public slots:
    void addScene(bool);
    void removeScene(Scene*);
    void removeAllScenes(bool);
    void stopAllScenes(bool);

    void removeSound(Player*);
    void addTrack(bool);
    void removeAllTracks(bool);
    void stopAllTracks(bool);

private:
    Ui::Widget *ui;

    QVector<Scene*> scenes;
    QVector<Player*> players;

    QGridLayout *soundGrid;
    QGridLayout *sceneGrid;

    QWidget *soundPanel;
    QWidget *scenePanel;

    QHBoxLayout *soundPanelLayout;
    QHBoxLayout *scenePanelLayout;

    QHBoxLayout *hSoundLayout;
    QVBoxLayout *vSoundLayout;

    QHBoxLayout *hSceneLayout;
    QVBoxLayout *vSceneLayout;

    int soundCol;
    int soundRow;

    int sceneCol;
    int sceneRow;

    QPushButton *addTrackButton;
    QPushButton *removeAllTracksButton;
    QPushButton *stopAllTracksButton;

    QPushButton *addSceneButton;
    QPushButton *removeAllScenesButton;
    QPushButton *stopAllScenesButton;
};

#endif // WIDGET_H
