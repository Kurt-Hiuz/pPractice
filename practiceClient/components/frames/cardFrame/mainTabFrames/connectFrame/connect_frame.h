#ifndef CONNECTFRAME_H
#define CONNECTFRAME_H

#include "../../I_cardframe.h"
#include "mainwindow.h"         //  родительский ui
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ConnectFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ConnectFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

signals:
    void signalTurnOnInterface();

private:
    QLineEdit *IPLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *connectToServerPuchButton;
    MainWindow *parentUi;

private slots:
    void slotConnectToServerPushButtonClicked();
};

#endif // CONNECTFRAME_H
