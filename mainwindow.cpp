@@ -0,0 +1,203 @@
#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Constructor */
MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->totalTimer = new QTimer ();
    this->counter = new QTimer();
    this->lapsTime = new QTime (0,0,0,0);
    this->totalTime = new QTime (0,0,0,0);
    this->catchedTime = new QTime (0,0,0,0);

    /* GridLayout : 1 */
    ui->stateLabel->setStyleSheet("font-size:11pt;margin-top:15px;");
    ui->exitButton->setStyleSheet("margin-top:20px;border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");

    /* GridLayout : 2 */
    ui->chronoTitleLabel->setStyleSheet("font-size:11pt;");
    ui->startButton->setStyleSheet("border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");
    ui->stopButton->setStyleSheet("border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");
    ui->totalTimeLabel->setStyleSheet("font-size:11pt;");
    ui->actualTimeLabel->setStyleSheet("font-size:11pt;");
    ui->lapButton->setStyleSheet("border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");
    ui->resetButton->setStyleSheet("border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");

    /* GridLayout : 3 */
    ui->minuteurLabel->setStyleSheet("font-size:11pt;");
    ui->timerSpin->setStyleSheet("font-size:11pt;""font-family:arial;""padding-left:3px;");
    ui->timerSpin->setMinimum(0);
    ui->timerSpin->setMaximum(20);

    // default value given in the capture
    // ui->totalTimeLabel->setText("00:00:14:846");
    // ui->actualTimeLabel->setText("00:00:03:361");
    // ui->timerSpin->setValue(12);
    // ui->progressBar->setValue(12);*/

    ui->actualCountLabel->setStyleSheet("font-size:11pt;");

    /* Progress Bar */
    QString style = QString( "QProgressBar{ border:1px solid grey; border-radius:3px; background-color:#e0e2e4; text-align:top; } QProgressBar::chunk{ background-color:#338AFF; }" );
    ui->progressBar->setStyleSheet(style);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(20);
    ui->progressBar->setTextVisible(false);
    ui->countDownButton->setStyleSheet("border:1px solid grey;""border-radius:2px;""background-color:white;""font-size:11pt;");

    QObject::connect(totalTimer, SIGNAL(timeout()), this, SLOT(UpdateTotalTime()));
    QObject::connect(counter, SIGNAL(timeout()), this, SLOT(UpdateCounter()));

    /*Set sound effect source*/
    soundeffect.setSource( QUrl("qrc:/timer/effect/timerEffect.wav") );

    //effect.setVolume(0.25f);
}

/* "source" Time to use to set, "destination" Time to be set*/
void MainWindow::setUpTime( QTime* source , QTime* destination)
{
    destination->setHMS( source->hour(), source->minute(), source->second(), source->msec()  );
}

/* Destructor */
MainWindow::~MainWindow()
{
    delete ui;
    delete lapsTime;
    delete totalTimer;
    delete counter;
}

/* CLICK */
/* START : button */
void MainWindow::on_startButton_clicked()
{
    /* Dsable the start button  */
    ui->startButton->setEnabled(false);
    ui->resetButton->setEnabled( false );

    if( !ui->stopButton->isEnabled() )
    {
        ui->stopButton->setEnabled(true);
    }

    ui->lapButton->setEnabled(true);

    this->totalTimer->start(3);
}

/* STOP : button */
void MainWindow::on_stopButton_clicked()
{
    this->totalTimer->stop();

    ui->stopButton->setEnabled(false);
    ui->lapButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    ui->resetButton->setEnabled(true);

    /* On a stop click we should update the lastCaught time before the next lap*/
    setUpTime( totalTime, catchedTime );
}

/* EXIT : button */
void MainWindow::on_exitButton_clicked()
{
    /* Make sure the is no running task before exit */
    if( totalTimer->isActive() || counter->isActive() )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Exit", "Sure?\n There is running task.", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QApplication::quit();
        }
    }
    else
    {
        QApplication::quit();
    }
}

/* RESET : button */
void MainWindow::on_resetButton_clicked()
{
    /* Reset the total time and update the time shower */
    this->totalTime->setHMS(0,0,0,0);
    ui->totalTimeLabel->setText( totalTime->toString("hh:mm:ss:zzz"));

    this->catchedTime->setHMS(0,0,0,0);
    ui->actualTimeLabel->setText( catchedTime->toString("hh:mm:ss:zzz"));
}

/* LAP : button */
void MainWindow::on_lapButton_clicked()
{
    QTime* oldCatchedTime = new QTime ();

    /* Set up the oldCatched time with the last caught time */
    setUpTime( catchedTime, oldCatchedTime );
     /* Catch the actual time */
    setUpTime( totalTime, catchedTime );

    QTime* laps = new QTime(0,0,0,0);

    int lapsInMilliseconde = oldCatchedTime->msecsTo( *catchedTime );
    *laps = laps->addMSecs( lapsInMilliseconde );

    ui->actualTimeLabel->setText( laps->toString("hh:mm:ss:zzz"));
}

/* COUNTDOWN : button */
void MainWindow::on_countDownButton_clicked()
{
    /* Make sure value is't 0 before starting the counter */
    if( ui->progressBar->value() !=0 )
    {
        ui->countDownButton->setEnabled(false);
        ui->timerSpin->setEnabled(false);
        this->counter->start(1000);
    }
}

/* UPDATE */
/* TOTALTIMELABEL : label */
void MainWindow::UpdateTotalTime()
{
    *totalTime = totalTime->addMSecs(3);
    ui->totalTimeLabel->setText( totalTime->toString("hh:mm:ss:zzz"));
}

/* ACTUALCOUNTLABEL : label */
void MainWindow::UpdateCounter()
{
    ui->progressBar->setValue( ui->progressBar->value() -1 );
    ui->actualCountLabel->setNum( ui->progressBar->value() );

    /*The sound effect*/
    soundeffect.play();

    if ( ui->progressBar->value() == 0 )
    {
        counter->stop();
        ui->countDownButton->setEnabled(true);
        ui->timerSpin->setEnabled(true);
    }
}

/* CHANGE */
/* TIMERSPIN : spinbox */
void MainWindow::on_timerSpin_valueChanged(int arg1)
{
     ui->actualCountLabel->setNum(arg1);
     ui->progressBar->setValue(arg1);
}

/* PROGRESSBAR */
void MainWindow::on_progressBar_valueChanged(int value)
{
    ui->stateLabel->setText("minuteur sur "+QString::number( value )+" secondes ");
}
