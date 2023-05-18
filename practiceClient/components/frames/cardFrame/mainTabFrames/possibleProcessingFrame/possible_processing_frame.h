#ifndef POSSIBLEPROCESSINGFRAME_H
#define POSSIBLEPROCESSINGFRAME_H

#include "../../I_cardframe.h"
#include "mainwindow.h"         //  родительский ui
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

class PossibleProcessingFrame : public I_CardFrame
{
    Q_OBJECT
public:
    PossibleProcessingFrame(MainWindow *ui);

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

private:
    QComboBox *chooseProcessingComboBox;
    QPushButton *chooseProcessingPushButton;
    MainWindow *parentUi;
};

#endif // POSSIBLEPROCESSINGFRAME_H
