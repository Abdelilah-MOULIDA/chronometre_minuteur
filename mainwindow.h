@@ -0,0 +1,59 @@
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QSound>
#include <QSoundEffect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /* Constructor */
        explicit MainWindow(QWidget *parent = nullptr);

        void setUpTime( QTime* source , QTime* destination);

        /* Destructor */
        ~MainWindow();

    private slots:
        /* CLICK */
        void on_startButton_clicked();
        void on_stopButton_clicked();
        void on_exitButton_clicked();
        void on_resetButton_clicked();
        void on_lapButton_clicked();
        void on_countDownButton_clicked();

        /* UPDATE */
        void UpdateTotalTime();
        void UpdateCounter();

        /* CHANGE */
        void on_timerSpin_valueChanged(int arg1);
        void on_progressBar_valueChanged(int value);

    private:
        Ui::MainWindow *ui;
        int countLap = 1;

        QTimer* totalTimer;
        QTimer* counter;

        QTime* lapsTime;
        QTime* totalTime;
        QTime* catchedTime;

        QSoundEffect soundeffect;
};

#endif // MAINWINDOW_H
