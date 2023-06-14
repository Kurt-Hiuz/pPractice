#ifndef CHATFRAME_H
#define CHATFRAME_H

#include "../../I_cardframe.h"
#include "mainwindow.h"         //  родительский ui
#include <QLineEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChatFrame : public I_CardFrame
{
    Q_OBJECT
public:
    ChatFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void setEnabledInteface(bool value) override;
    void clearChat();

signals:
    void signalSendMessageToServer(QString message, QString senderName);

private:
    QLineEdit *callMeLineEdit;
    QTextBrowser *chatTextBrowser;
    QLineEdit *messageLineEdit;
    QPushButton *sendMessagePushButton;
    MainWindow *parentUi;

private slots:
    void slotSendMessagePushButton();
};

#endif // CHATFRAME_H
