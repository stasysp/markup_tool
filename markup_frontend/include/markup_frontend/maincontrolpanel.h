#ifndef MAINCONTROLPANEL_H
#define MAINCONTROLPANEL_H

#include <QWidget>
#include <QPushButton>

class MainControlPanel : public QWidget
{
    // Q_OBJECT
public:
    explicit MainControlPanel(QWidget *parent = nullptr);

signals:

private slots:
    // void load_file();

private:
    QPushButton *loadmodel = nullptr;
    QPushButton *loadvideo = nullptr;
    QPushButton *loadtracks = nullptr;
    QPushButton *run = nullptr;
    QPushButton *play = nullptr;
};

#endif // MAINCONTROLPANEL_H
