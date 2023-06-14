#ifndef MAINWINDOW_H
#define MAINWINDOW_H

///     Класс MainWindow определяет главную форму приложения
///     Переменные:
///         delimiter - строка разделитель для чата
///         * серверные:
///             socket - создаваемый сокет сервера
///             ui - сама форма
///             server - экземпляр класса сервера
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
///         * работа с файлами:
///             m_fileSystemModel - для работы с файловой моделью
///     Методы:
///         setServerSettingsFromFile - устанавливает настройки из файла
///         setFileSystemModel - устанавливает модель файловой системы
///     Сигналы:
///         signalDisconnectAll - сообщению серверу об отключении всех клиентов
///         signalSendMessage - обращение к серверу отправить сообщение
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
///         slotUpdateUiComboBox - обновление комбобокса
///         slotSettingsFileChanged - файл настроек изменён
///         slotClearEntryFolder - отчет о чистке /Entry
///         slotChatNewMessage - обработка нового сообщения
///         on_restartServerPushButton_clicked - перезапуск сервера
///         on_sendMsgPushButton_clicked - отправка сообщения
///         on_serverMessageLineEdit_returnPressed - отправка сообщения по нажатию Enter

///  ========================    классы для работы сервера
#include <QTcpServer>           //  Работа с сервером
#include <QTcpSocket>           //  работа с сокетами
///  ========================
///
///  ========================   классы для работы с файлаи
#include <QFileDialog>          //  вызов диалогового окна для файлов
#include <QFile>                //  работа с файлами
#include <QDir>                 //  работа с директориями
#include <QFileSystemModel>     //  работа с моделью файлов
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
#include <QVBoxLayout>          //  вертикальное позиционирование
///  ========================
///
///  ========================   классы проекта
#include "components/frames/cardFrame/I_cardframe.h"    //  интерфейс работы с карточками настройки
#include "helperClasses/jsonPacker/json_packer.h"       //  упаковщик карточки в json вариант
#include "helperClasses/managers/workspaceManager/workspace_manager.h"  //  менеджер рабочего пространства
#include "components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.h"    //  фрейм выбора рабочей директории
#include "components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.h" //  фрейм возможных обработок
#include "components/frames/cardFrame/changePortSpinBoxFrame/change_port_spinbox_frame.h"   //  фрейм смены порта
#include "components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.h" //  фрейм макс. подключений
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

    QFileSystemModel *m_fileSystemModel;

    void setServerSettingsFromFile(const QString &filePath);
    void setFileSystemModel(QString folderPath);

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
    void slotUpdateUiComboBox(const QString &fileName);
    void slotSettingsFileChanged(const QString &filePath);
    void slotClearEntryFolder(QString message);
    void slotChatNewMessage(QString message);
    void on_restartServerPushButton_clicked();
    void on_sendMsgPushButton_clicked();
    void on_serverMessageLineEdit_returnPressed();

signals:
    void signalSocketDisplayed(QTcpSocket* displayedSocket);
    void signalDisconnectSocket(int socketDiscriptor);
    void signalUpdatePossibleProcessing(QVariant newPossibleProcessingData);
    void signalDisconnectAll(QString reason);
    void signalSendMessage(QString message);
};

#endif // MAINWINDOW_H
