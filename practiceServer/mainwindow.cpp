// #include "components/frames/cardFrame/cardframe.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

#include "components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.h"
#include "components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.h"
#include "components/frames/cardFrame/changePortLineEditFrame/change_port_line_edit_frame.h"
#include "components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool server_started = false;    //  изначально наш сервер выключен

    server = new Server(server_started);    //  создаем сервер, передавая его состояние, которое изменится в конструкторе

    if(server_started)  //  проверка состояния "true"
        {
            ui->infoAboutServerTextEdit->append("Сервер запущен на ["+server->serverAddress().toString()+" IP] и "+QString::number(server->generatedServerPort)+" порту");  //  уведомление
        }
        else
        {
            ui->infoAboutServerTextEdit->append("Сервер не запущен");   //  уведомление
            return;
        }

    connect(server, &Server::signalStatusServer, this, &MainWindow::slotStatusServer);  //  связка для отображения статуса сервера, вывод в консоль
    connect(server, &Server::signalAddSocketToListWidget, this, &MainWindow::slotAddSocketToListWidget);    //  связка для отображения добавления клиентов в clientsListWidget
    connect(server, &Server::signalDeleteSocketFromListWidget, this, &MainWindow::slotDeleteSocketFromListWidget);  //  связка для удаления сокета из clientsListWidget
    connect(this, &MainWindow::signalNewWorkspaceFolder, server, &Server::slotNewWorkspaceFolder);  //  связка для отображения новой директории
    connect(this, &MainWindow::signalSocketDisplayed, server, &Server::slotSocketDisplayed);    //  связка для отправки подключившемуся сокету список доступных обработок
    connect(this, &MainWindow::signalDisconnectSocket, server, &Server::slotDisconnectSocket);  //  связка для принудительного удаления сокета
    connect(this, &MainWindow::signalUpdatePossibleProcessing, server, &Server::slotUpdatePossibleProcessing);  //  связка для обновления списка обработок у клиентоав

    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

    QVBoxLayout *settingsContainer = new QVBoxLayout();

    workspaceManager = new WorkspaceManager();
    connect(workspaceManager, &WorkspaceManager::signalUpdateUiComboBox, this, &MainWindow::updateUiComboBoxSlot);
    connect(workspaceManager, &WorkspaceManager::signalStatusServer, this, &MainWindow::slotStatusServer);  //  связка для отображения статуса сервера, вывод в консоль

    m_selectWorkspaceFrame = new SelectWorkspaceFrame(this);
    m_possibleProcessingFrame = new PossibleProcessingComboBoxFrame(this);
    m_changePortLineEditFrame = new ChangePortLineEditFrame(this);
    m_maxConnectionSpinBoxFrame = new MaxConnectionSpinBoxFrame(this);

    m_selectWorkspaceFrame->createInterface();
    m_possibleProcessingFrame->createInterface();
    m_changePortLineEditFrame->createInterface();
    m_maxConnectionSpinBoxFrame->createInterface();

    settingsContainer->addWidget(m_selectWorkspaceFrame);
    settingsContainer->addWidget(m_possibleProcessingFrame);
    settingsContainer->addWidget(m_changePortLineEditFrame);
    settingsContainer->addWidget(m_maxConnectionSpinBoxFrame);

    ui->settingsFrame->setLayout(settingsContainer);

    // устанавливаем специальную политику отображения меню
    ui->clientsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // ждем сигнала для отображения меню
    connect(ui->clientsListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotShowContextMenu(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotStatusServer(QString status)   //  обработчик состояния
{
    qDebug() << "MainWindow::slotStatusServer:      " << status; //  вывод в консоль статуса
    ui->infoAboutServerTextEdit->append(delimiter+QTime::currentTime().toString()+" | <font color = black><\\font>"+status);    //  и также в textEdit
}

void MainWindow::slotAddSocketToListWidget(QTcpSocket *socketToAdd)
{
    //  TODO:   сделать обращение к clientsListWidget и добавление данных с сокета
    ui->clientsListWidget->addItem("User desc:"+QString::number(socketToAdd->socketDescriptor())+" | IP: "+socketToAdd->localAddress().toString());
//    qDebug() << QString::number(socketToAdd->socketDescriptor()) << socketToAdd->localAddress().toString();
    MainWindow::signalSocketDisplayed(socketToAdd);
}

void MainWindow::slotDeleteSocketFromListWidget(QMap<QTcpSocket *, QString> mapSockets)
{
//    qDebug() << "User desc :"+QString::number(mapSockets->socketDescriptor())+" | IP: "+mapSockets->localAddress().toString();
//    for(int i = 0; i < ui->clientsListWidget->count(); i++){    //  перебираем все элементы clietnsListWidget
//        //  ↓↓↓ Если текст элемента совпадает с удаляемым сокетом, ....
//        if(ui->clientsListWidget->item(i)->text() == "User desc:"+QString::number(mapSockets->socketDescriptor())+" | IP: "+mapSockets->localAddress().toString()){
//            QListWidgetItem* itemSocketToDelete = ui->clientsListWidget->takeItem(i);   //  ...., то удаляем из clientsListWidget сокет
//            delete itemSocketToDelete;  //  но он останется в памяти, поэтому его надо удалить вручную по совету документации
//            break;
//        }
//    }
    ui->clientsListWidget->clear();
    for(auto itemSocket = mapSockets.begin(); itemSocket != mapSockets.end(); itemSocket++){
        ui->clientsListWidget->addItem("User desc:"+QString::number(itemSocket.key()->socketDescriptor())+" | IP: "+itemSocket.key()->localAddress().toString());
    }

}

void MainWindow::slotShowContextMenu(const QPoint &pos)
{
    QPoint globalPos;
    if (sender()->inherits("QAbstractScrollArea"))
      globalPos = dynamic_cast<QAbstractScrollArea*>(sender())->viewport()->mapToGlobal(pos);
    else
      globalPos = dynamic_cast<QWidget*>(sender())->mapToGlobal(pos);

    // Создаем меню
    QMenu menu;
    // Создаем пункт меню
    QAction* action1 = new QAction(QString::fromUtf8("Отключить"), this);
    // добавляем пункт в меню
    menu.addAction(action1);
}

void MainWindow::slotDisconnectClient()
{
    //  определяем выбранную строчку
    int row = ui->clientsListWidget->selectionModel()->currentIndex().row();
    // проверяем, действительно ли выбрали
    if(row >= 0){
        //  если да, то выводим предупреждение
        if (QMessageBox::warning(this,
                                 QString::fromUtf8("Отключение сокета"),
                                 QString::fromUtf8("Вы уверены, что хотите отключить клиента?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            //  при отказе выходим
            return;
        } else {
            //  иначе берем дескриптор сокета
            QString socketText = ui->clientsListWidget->currentItem()->text().remove(0, ui->clientsListWidget->currentItem()->text().indexOf(":")+1);
            socketText = socketText.remove(4, socketText.length());

            //  и отправляем запрос на сервер, чтобы по нему удалили сокет
            MainWindow::signalDisconnectSocket(socketText.toInt());
            qDebug() << "MainWindow::slotDisconnectClient:      descriptor to delete: " << socketText;
        }
    }
}

void MainWindow::on_chooseWorkspaceDirPushButton_clicked()   //  по нажатию на "Choose save directory"
{
    QString folderPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");  //  выбираем папку
    if(!folderPath.isEmpty()){
        //  для наглядности работы сохраняем путь в информационный QLabel
        //  при вызове setValue данный виджет сам вызовет сигнал для установки директории на сервере
        m_selectWorkspaceFrame->setValue(folderPath);
        ui->infoAboutServerTextEdit->append(m_selectWorkspaceFrame->getValue().firstKey());

        qDebug() << "MainWindow::on_chooseWorkspaceDirPushButton_clicked:   " << folderPath;

        //  теперь можно сохранить настройки
        ui->saveSettingsPushButton->setEnabled(true);

        workspaceManager->setRootFolder(folderPath);
        if(workspaceManager->createWorkspaceFolders()){
            ui->infoAboutServerTextEdit->append("<hr/>Рабочая папка организована!");
            for(auto item : ui->settingsFrame->children()){

                //  если встречаем слой выравнивания, то пропускаем
                //  поскольку в списке настроек первым элементом всегда будет слой выравнивания
                if(QString(item->metaObject()->className()).contains("Layout") ||
                        item->objectName().contains("Choose workspace directory Frame")){
                    continue;
                }

                //  включаем интерфейс
                dynamic_cast<I_CardFrame*>(item)->enableInteface();
            }
        } else {
            ui->infoAboutServerTextEdit->append("<hr/>Рабочая папка не организована!");
        }
    }
}

void MainWindow::on_clientsListWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "MainWindow::on_clientsListWidget_customContextMenuRequested:       contextMenu clicked";

    //  Создаем объект контекстного меню
    QMenu* menu = new QMenu(this);
    //  Создаём действия для контекстного меню
    QAction* disconnectClient = new QAction(QString::fromUtf8("Отключить"), this);
    //  Подключаем СЛОТы обработчики для действий контекстного меню
    connect(disconnectClient, SIGNAL(triggered()), this, SLOT(slotDisconnectClient()));     // Обработчик вызова диалога редактирования
    //  Устанавливаем действия в меню
    menu->addAction(disconnectClient);
    //  Вызываем контекстное меню
    menu->popup(ui->clientsListWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::on_openJSONSettingsFilePushButton_clicked()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Выбор файла", "C:\\");   //  открываем диалоговое окно с заголовком "Выбор файла" и по умолчанию ставим путь C:/
    QFile fileJSONSetting;
    fileJSONSetting.setFileName(filePath);
    fileJSONSetting.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = fileJSONSetting.readAll();
    fileJSONSetting.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8(), &error);
    qDebug() << "MainWindow::on_openJSONSettingsFilePushButton_clicked:     Error: " << error.errorString() << error.offset << error.error;

    QJsonObject documentObject = document.object();
    QString keyObject = ""; //  ключ всегда строка
    QVariant valueObject;  //  а вот значение может быть разным

    for (int i = 0; i < documentObject.size(); i++) { //  проходимся по всему файлу

        keyObject = documentObject.keys().at(i);  //  берем i-тый ключ-название_виджета

        for(auto settingsTabChild : ui->settingsFrame->children()){
            if(settingsTabChild->metaObject()->className() != keyObject) continue;

            //  получаем значение по ключу
            valueObject = documentObject.value(keyObject);

            dynamic_cast<I_CardFrame*>(settingsTabChild)->setValue(valueObject);

            //  получаем строку для консоли и добавляем её в infoAboutServerTextEdit
            ui->infoAboutServerTextEdit->append(dynamic_cast<I_CardFrame*>(settingsTabChild)->getValue().firstKey());

            break;
        }

    }

    ui->infoAboutServerTextEdit->append("<hr/>Настройки загружены");

    qDebug() << "======";
    qDebug() << "Настройки установлены";
}

void MainWindow::on_saveSettingsPushButton_clicked()
{
    //  проходимся по сгенерированному списку настроек
    for(auto item : ui->settingsFrame->children()){

        //  если встречаем слой выравнивания, то пропускаем
        //  поскольку в списке настроек первым элементом всегда будет слой выравнивания
        if(QString(item->metaObject()->className()).contains("Layout")){
            continue;
        }

        //  получаем json вариант данных с элемента, приведённого от QObject* к I_CardFrame*
        m_currentJsonValue = m_jsonPacker.getJsonVersionValue(dynamic_cast<I_CardFrame*>(item));

        //  выводим в консоль сообщение, получая первый ключ
        ui->infoAboutServerTextEdit->append(dynamic_cast<I_CardFrame*>(item)->getValue().firstKey());

        //  дополняем в m_currentJsonObject ключ(название класса) : значение(json вариант)
        m_currentJsonObject[item->metaObject()->className()] = m_currentJsonValue;
    }

    //  добавляем в консоль отчет по сохранению настроек
    ui->infoAboutServerTextEdit->append(workspaceManager->saveSettings(m_currentJsonObject));
}

void MainWindow::updateUiComboBoxSlot(const QString &fileName)
{
    qDebug() << "MainWindow::updateUiComboBoxSlot !!!   " << fileName;

    QFile fileJSONSetting;
    fileJSONSetting.setFileName(fileName);

    if(!fileJSONSetting.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->infoAboutServerTextEdit->append("<font color = red><\\font>Не удалось открыть файл списка обработок!<font color = black><\\font>");
        return;
    }
    QString val = fileJSONSetting.readAll();
    fileJSONSetting.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8(), &error);
    qDebug() << "MainWindow::updateUiComboBoxSlot:     Error: " << error.errorString() << error.offset << error.error;

    QJsonObject documentObject = document.object();
    QString keyObject = ""; //  ключ всегда строка
    QVariant valueObject;  //  а вот значение может быть разным

    for (int i = 0; i < documentObject.size(); i++) { //  проходимся по всему файлу

        keyObject = documentObject.keys().at(i);  //  берем i-тый ключ-название_виджета

        //  получаем значение по ключу
        valueObject = documentObject.value(keyObject);

        ui->settingsFrame->findChild<PossibleProcessingComboBoxFrame*>("Possible processing Frame")->setValue(valueObject);
    }

    ui->infoAboutServerTextEdit->append("<hr/>Список обновлен");

    emit signalUpdatePossibleProcessing(valueObject);

    qDebug() << "MainWindow::updateUiComboBoxSlot:  ======";
    qDebug() << "MainWindow::updateUiComboBoxSlot:  Настройки установлены";
}

