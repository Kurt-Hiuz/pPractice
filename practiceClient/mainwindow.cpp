#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "components/frames/cardFrame/mainTabFrames/connectFrame/connect_frame.h" //  карточка подключения
#include "components/frames/cardFrame/mainTabFrames/possibleProcessingFrame/possible_processing_frame.h"  //  карточка обработок
#include "components/frames/cardFrame/mainTabFrames/chatFrame/chat_frame.h"   //  карточка чата
#include "components/frames/cardFrame/mainTabFrames/fileFrame/file_frame.h"   //  карточка файла
#include "components/frames/cardFrame/settingsTabFrames/selectWorkspaceFrame/select_workspace_frame.h"  //  карточка выбора рабочей папки

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("D.I.P. Client++");

    connectFrame = new ConnectFrame(this);

    possibleProcessingFrame = new PossibleProcessingFrame(this);
    chatFrame = new ChatFrame(this);
    fileFrame = new FileFrame(this);

    selectWorkspaceFrame = new SelectWorkspaceFrame(this);

    connectFrame->createInterface();
    possibleProcessingFrame->createInterface();
    chatFrame->createInterface();
    fileFrame->createInterface();

    selectWorkspaceFrame->createInterface();

    mainContainer->addWidget(connectFrame);
    mainContainer->addWidget(possibleProcessingFrame);
    mainContainer->addWidget(chatFrame);
    mainContainer->addWidget(fileFrame);

    settingsContainer->addWidget(selectWorkspaceFrame);

    ui->mainFrame->setLayout(mainContainer);
    ui->settingsMainFrame->setLayout(settingsContainer);

    workspaceManager = new WorkspaceManager();
    connect(workspaceManager, &WorkspaceManager::signalStatusClient, this, &MainWindow::slotStatusClient);  //  связка для отображения статуса клиента, вывод в консоль

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEnableInteface()
{
    QMap<QString, QVariant> mapValue = connectFrame->getValue();

    int port = mapValue["portLineEdit"].toInt();
    QString IP = mapValue["IPLineEdit"].toString();

    if(port < 1024){
        fileFrame->setValue("Проверьте IP и порт сервера! Вы не подключились");
        return;
    }
    if(client == nullptr){
        client = new Client();
    }

    client->connectToHost(IP, port);   //  подключение к серверу (локальный адрес + порт такой же, как у сервера)

    if(client->state() != 2){
        chatFrame->setValue("Ошибка подключения"+delimiter);
        return;
    }

    for(auto mainTabChild : ui->mainFrame->children()){
        if(QString(mainTabChild->metaObject()->className()).contains("Layout")){
            continue;
        }
        dynamic_cast<I_CardFrame*>(mainTabChild)->setEnabledInteface(true);
    }

    connect(this, &MainWindow::signalSendTextToServer, client, &Client::slotSendTextToServer);
    connect(this, &MainWindow::signalSendFileToServer, client, &Client::slotSendFileToServer);
    connect(this, &MainWindow::signalSendToServer, client, &Client::slotSendToServer);
    connect(workspaceManager, &WorkspaceManager::signalSetClientFolders, client, &Client::slotSetClientFolders);
    connect(client, &Client::signalStatusClient, this, &MainWindow::slotStatusClient);
    connect(client, &Client::signalMessageTextBrowser, this, &MainWindow::slotMessageTextBrowser);
    connect(client, &Client::signalSetCBDataForm, this, &MainWindow::slotSetCBDataForm);
    connect(client, &Client::signalSetFilePathLabel, this, &MainWindow::slotSetFilePathLabel);
    connect(client, &Client::signalEnableInterface, this, &MainWindow::slotEnableInterface);

    chatFrame->setValue("Вы подключились!"+delimiter);

    possibleProcessingFrame->setEnabledInteface(false);
    fileFrame->setEnabledInteface(false);
}

void MainWindow::slotStatusClient(QString status)
{
    ui->consoleTextBrowser->append(QTime::currentTime().toString()+" | "+status+"</br>");
}

void MainWindow::slotMessageTextBrowser(QString message)
{
    chatFrame->setValue(message);
}

void MainWindow::slotSetCBDataForm(QMap<QString, QVariant> possibleProcessingData)
{
    possibleProcessingFrame->setValue(possibleProcessingData);
}

void MainWindow::slotSetFilePathLabel(QString text)
{
    fileFrame->setValue(text);
}

void MainWindow::slotEnableInterface(QString message)
{
    for(auto mainTabChild : ui->mainFrame->children()){
        if(QString(mainTabChild->metaObject()->className()).contains("Layout")){
            continue;
        }
        //  выключаем весь интерфейс
        dynamic_cast<I_CardFrame*>(mainTabChild)->setEnabledInteface(false);
    }
    client->disconnectFromHost();
    client = nullptr;
    //  включаем возможность подключения
    connectFrame->setEnabledInteface(true);
    chatFrame->setValue(message);
}

void MainWindow::on_chooseWorkspaceDirPushButton_clicked(){
    QString folderPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");  //  выбираем папку
    if(!folderPath.isEmpty()){
        //  для наглядности работы сохраняем путь в информационный QLabel
        //  при вызове setValue данный виджет сам вызовет сигнал для установки директории на сервере
        selectWorkspaceFrame->setValue(folderPath);
        ui->consoleTextBrowser->append(selectWorkspaceFrame->getValue().firstKey());

        workspaceManager->setRootFolder(folderPath);
        client->setWorkspaceManager(workspaceManager);
        if(workspaceManager->createWorkspaceFolders()){
            ui->consoleTextBrowser->append("<br/>Рабочая папка организована!");
            //  создаем наблюдатель за папкой Entry
            ui->consoleTextBrowser->append(workspaceManager->setEntryWatcher());
            ui->consoleTextBrowser->append(workspaceManager->setProcessedWatcher());

            //  включаем выбор обработок
            possibleProcessingFrame->setEnabledInteface(true);

            //  и включаем выбор файла для отправки
            fileFrame->setEnabledInteface(true);
        } else {
            ui->consoleTextBrowser->append("<hr/>Рабочая папка не организована!");
        }
    }
}

void MainWindow::on_chooseProcessingPushButton_clicked()
{
    emit signalSendToServer("Set processing on client", dynamic_cast<PossibleProcessingFrame*>(possibleProcessingFrame)->getCurrentData());   //  отправляем серверу текущий текст в комбобоксе
    ui->consoleTextBrowser->append("Выбрано: "+dynamic_cast<PossibleProcessingFrame*>(possibleProcessingFrame)->getCurrentData()+delimiter);   //  пишем клиенту, что он выбрал
}
