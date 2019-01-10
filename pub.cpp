#include "Pub.h"
#include <QMessageBox>


void showErrorMessage(QString message)
{
    QMessageBox msg(NULL);
    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

int showQueryMessage(QString message)
{
    int ret = -1;
    QMessageBox msg(NULL);
    msg.setWindowTitle("Query");
    msg.setIcon(QMessageBox::Question);
    msg.setText(message);
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel );

    ret = msg.exec();
    return ret;
}
