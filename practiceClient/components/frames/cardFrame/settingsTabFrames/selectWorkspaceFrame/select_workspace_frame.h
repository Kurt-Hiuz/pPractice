#ifndef SELECTWORKSPACEFRAME_H
#define SELECTWORKSPACEFRAME_H

#include "../../I_cardframe.h"
#include "mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVariant>

class SelectWorkspaceFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectWorkspaceFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

private:
    QPushButton *chooseWorkspaceDirPushButton;
    QLabel *dataLabel;
    MainWindow *parentUi;
    QString consoleMessage;
};

#endif // SELECTWORKSPACEFRAME_H
