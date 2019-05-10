#ifndef MAINCONTROLPANEL_H
#define MAINCONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QDir>
#include <QComboBox>

class MainControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MainControlPanel(QWidget *parent = nullptr);

signals:
    void send_video_path(QDir dir);
    void send_tracks_path(QString path);
    void send_savetracks(QString path);
    void send_run();
    void send_move_all_frames(int move);

private slots:
    // void slot_loadmodel();
    void slot_loadvideo();
    void slot_loadtracks();
    void slot_savetracks();
    void slot_run();
    void slot_play();
    void slot_step_forward();
    void slot_step_backward();

private:
    //QComboBox *selectmodel = nullptr;
    QPushButton *loadvideo = nullptr;
    QPushButton *loadtracks = nullptr;
    QPushButton *savetracks = nullptr;
    QPushButton *run = nullptr;
    QPushButton *play = nullptr;
    QPushButton *step_forward = nullptr;
    QPushButton *step_backward = nullptr;
};

#endif // MAINCONTROLPANEL_H
