#ifndef SELECTPROCESSORFRAME_H
#define SELECTPROCESSORFRAME_H

#include "../../I_cardframe.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariant>

class SelectProcessorFrame : public I_CardFrame
{
    Q_OBJECT
public:
    SelectProcessorFrame();

    void createInterface() override;
    QMap<QString, QVariant> getValue() override;
    void setValue(QVariant value) override;
    void switchEnabledInteface() override;

private:
    QPushButton *chooseProcessorPushButton;
    QLabel *dataLabel;
};

#endif // SELECTPROCESSORFRAME_H
