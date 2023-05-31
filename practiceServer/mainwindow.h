#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///     Класс MainWindow определяет главную форму приложения
///     Переменные:
///         delimiter - строка разделитель для чата
///         * серверные:
///             socket - создаваемый сокет сервера
///             ui - сама форма
///             server - экземпляр класса сервера
///             nextBlockSize - размер блока сообщения
///         * менеджеры:
///             workspaceManager - отвечает за работу с рабочей директорией
///         * графические компоненты:
///             m_selectWorkspaceFrame - фрейм выбора рабочей папки
///             m_possibleProcessingFrame - фрейм возможных обработок
///             m_changePortLineEditFrame - фрейм смены порта сервера
///             m_maxConnectionSpinBoxFrame - фрейм смены максимального количества подключений
///         * работа с JSON:
///             m_jsonPacker - упаковщик в json формат графические компоненты
///             m_currentJsonObject - для работы с json объектами
///             m_currentJsonValue - для работы с json значениями
///             m_currentJsonArray - для работы с json массивами
///     Сигналы:
///         signalNewWorkspaceFolder - установка новой рабочей папки на сервере
///         signalSocketDisplayed - сообщение серверу, что клиент отобразился
///         signalDisconnectSocket - сообщение серверу, что клиент убран
///         signalUpdatePossibleProcessing - сообщение серверу о новых обработках
///     Слоты:
///         slotStatusServer - отображение статуса сервера
///         slotAddSocketToListWidget - отображение добавляемых сокетов
///         slotDeleteSocketFromListWidget - сигнал для принудительного удаления сокетов
///         slotShowContextMenu - отображение контекстного меню
///         slotDisconnectClient - обработка отключения клиента
///         on_chooseWorkspaceDirPushButton_clicked - по нажатию на "Выбрать рабочую папку"
///         on_clientsListWidget_customContextMenuRequested - отображение контекстного меню
///         on_openJSONSettingsFilePushButton_clicked - по нажатию на "Открыть файл настроек"
///         on_saveSettingsPushButton_clicked - по нажатию на "Сохранить настройки"
///         updateUiComboBoxSlot - обновление комбобокса

///  ========================    классы для работы сервера
#include <QTcpServer>           //  Работа с сервером
#include <QTcpSocket>           //  работа с сокетами
///  ========================
///
///  ========================   классы для работы с файлаи
#include <QFileDialog>          //  вызов диалогового окна для файлов
#include <QFile>                //  работа с файлами
#include <QDir>                 //  работа с директориями
///  ========================
///
///  ========================   классы для работы с json
#include <QJsonObject>          //  работа с json объектами
#include <QJsonArray>           //  работа с json массивами
#include <QJsonDocument>        //  работа с json документами
#include <QJsonValue>           //  работа с json значениями
#include <QJsonParseError>      //  парсинг json Ошибок
///  ========================
///
///  ========================   классы для работы с элементами формы
#include <QMainWindow>          //  работа главного окна
#include <QListWidgetItem>      //  список виджетов
#include <QAbstractScrollArea>  //  работа со скролящимися областями
#include <QMessageBox>          //  работа со всплывающими окнами
#include <QFileSystemModel>     //  модель файловой системы
#include <QTreeView>            //  отображение в виде дерева
///  ========================
///
///  ========================   классы проекта
#include "components/frames/cardFrame/I_cardframe.h"    //  интерфейс работы с карточками настройки
#include "helperClasses/jsonPacker/json_packer.h"       //  упаковщик карточки в json вариант
#include "helperClasses/managers/workspaceManager/workspace_manager.h"  //  менеджер рабочего пространства
#include "server.h"             //  работа с сервером
///  ========================

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTcpSocket *socket;

private:
    Ui::MainWindow *ui;
    Server* server;
    WorkspaceManager *workspaceManager;

    I_CardFrame *m_selectWorkspaceFrame;
    I_CardFrame *m_possibleProcessingFrame;
    I_CardFrame *m_changePortLineEditFrame;
    I_CardFrame *m_maxConnectionSpinBoxFrame;

    JsonPacker m_jsonPacker;

    QString delimiter = "<font color = black><\\font>=======================<br>";

    QJsonObject m_currentJsonObject;
    QJsonValue m_currentJsonValue;
    QJsonArray m_currentJsonArray;

    quint16 nextBlockSize;

public slots:
    void slotStatusServer(QString status);
    void slotAddSocketToListWidget(QTcpSocket* socketToAdd);
    void slotDeleteSocketFromListWidget(QMap<QTcpSocket*, QString> mapSockets);
    void slotShowContextMenu(const QPoint &pos);
    void slotDisconnectClient();
    void on_chooseWorkspaceDirPushButton_clicked();

private slots:
    void on_clientsListWidget_customContextMenuRequested(const QPoint &pos);
    void on_openJSONSettingsFilePushButton_clicked();
    void on_saveSettingsPushButton_clicked();
    void updateUiComboBoxSlot(const QString &fileName);

signals:
    void signalSocketDisplayed(QTcpSocket* displayedSocket);
    void signalDisconnectSocket(int socketDiscriptor);
    void signalUpdatePossibleProcessing(QVariant newPossibleProcessingData);
};

#endif // MAINWINDOW_H
