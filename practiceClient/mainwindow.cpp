#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "components/frames/cardFrame/mainTabFrames/connectFrame/connect_frame.h" //  карточка подключения
#include "components/frames/cardFrame/mainTabFrames/possibleProcessingFrame/possible_processing_frame.h"  //  карточка обработок
#include "components/frames/cardFrame/mainTabFrames/chatFrame/chat_frame.h"   //  карточка чата
#include "components/frames/cardFrame/mainTabFrames/fileFrame/file_frame.h"   //  карточка файла
#include "components/frames/cardFrame/settingsTabFrames/selectWorkspaceFrame/select_workspace_frame.h"  //  карточка выбора рабочей папки
#include "components/frames/cardFrame/settingsTabFrames/selectProcessorFrame/select_processor_frame.h"  //  карточка выбора обработчика

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /// Глоссарий, описывающий тип отправляемого сообщения
    /// первое число определяет тип (0 - простой сигнал о чем-то \ 1 - запрос чего-то)
    /// второе число определяет конец какого-то действия, если оно в несколько этапов, например, передача файла
    /// третье и последующие числа определяют тип передаваемых данных

    mapRequest[""] = "";  //  ничего не нужно
    mapRequest["000"] = "Disconnect";   //  сигнал на отключение
    mapRequest["001"] = "Message";   //  отправляется простое сообщение
    mapRequest["0011"] = "Message from someone";    //  отправляется сообщение от кого-то конкретного
    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
    mapRequest["103"] = "Request part of processing file";  //  запрос на еще одну часть обрабатываемого файла
    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)
    mapRequest["004"] = "Possible treatments ComboBox data";    //  отправка данных по доступным обработкам
    mapRequest["0041"] = "Set treatment on client";     //  закрепление возможной обработки за сокетом


    nextBlockSize = 0;  //  обнуляем размер сообщения в самом начале работы

//    fileSystemWatcher = new QFileSystemWatcher;
//    fileSystemWatcher->addPath(rawInformationDirectory);    //  устанавливаем папку для слежки
//    connect(fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(slotFolderForRawInformationChanged(QString)));

    connectFrame = new ConnectFrame(this);

    possibleProcessingFrame = new PossibleProcessingFrame(this);
    chatFrame = new ChatFrame(this);
    fileFrame = new FileFrame(this);

    selectWorkspaceFrame = new SelectWorkspaceFrame(this);
    selectProcessorFrame = new SelectProcessorFrame();

    connectFrame->createInterface();
    possibleProcessingFrame->createInterface();
    chatFrame->createInterface();
    fileFrame->createInterface();

    selectWorkspaceFrame->createInterface();
    selectProcessorFrame->createInterface();

    mainContainer->addWidget(connectFrame);
    mainContainer->addWidget(possibleProcessingFrame);
    mainContainer->addWidget(chatFrame);
    mainContainer->addWidget(fileFrame);

    settingsContainer->addWidget(selectWorkspaceFrame);
    settingsContainer->addWidget(selectProcessorFrame);

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
    client = new Client();

    QMap<QString, QVariant> mapValue = connectFrame->getValue();

    client->connectToHost(mapValue["IPLineEdit"].toString(), mapValue["portLineEdit"].toUInt());   //  подключение к серверу (локальный адрес + порт такой же, как у сервера)
    if(!(client->isOpen())){
        fileFrame->setValue("Check your IP and host! You're offline");
        client = nullptr;
        return;
    }

    for(auto mainTabChild : ui->mainFrame->children()){
        if(QString(mainTabChild->metaObject()->className()).contains("Layout")){
            continue;
        }
        dynamic_cast<I_CardFrame*>(mainTabChild)->switchEnabledInteface();
    }

    connect(this, &MainWindow::signalSendTextToServer, client, &Client::slotSendTextToServer);
    connect(this, &MainWindow::signalSendFileToServer, client, &Client::slotSendFileToServer);
    connect(this, &MainWindow::signalSendToServer, client, &Client::slotSendToServer);
    connect(workspaceManager, &WorkspaceManager::signalSetClientFolders, client, &Client::slotSetClientFolders);
    connect(workspaceManager, &WorkspaceManager::signalSendProcessedFile, this, &MainWindow::slotSendProcessedFile);
    connect(client, &Client::signalStatusClient, this, &MainWindow::slotStatusClient);
    connect(client, &Client::signalMessageTextBrowser, this, &MainWindow::slotMessageTextBrowser);
    connect(client, &Client::signalSetCBDataForm, this, &MainWindow::slotSetCBDataForm);
    connect(client, &Client::signalSetFilePathLabel, this, &MainWindow::slotSetFilePathLabel);


    chatFrame->setValue("You are online!"+delimiter);
}

void MainWindow::slotStatusClient(QString status)
{
    ui->consoleTextBrowser->append(QTime::currentTime().toString()+" | "+status+"</hr>");
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

void MainWindow::slotSendProcessedFile(QString processedFileName)
{
    qDebug() << "MainWindow::slotSendProcessedFile  файл " << processedFileName;
    emit signalSendFileToServer(processedFileName);
}

void MainWindow::on_chooseWorkspaceDirPushButton_clicked(){
    QString folderPath = QFileDialog::getExistingDirectory(0, "Выбор папки", "");  //  выбираем папку
    if(!folderPath.isEmpty()){
        //  для наглядности работы сохраняем путь в информационный QLabel
        //  при вызове setValue данный виджет сам вызовет сигнал для установки директории на сервере
        selectWorkspaceFrame->setValue(folderPath);
        ui->consoleTextBrowser->append(selectWorkspaceFrame->getValue().firstKey());

        qDebug() << "MainWindow::on_chooseWorkspaceDirPushButton_clicked:   " << folderPath;

        workspaceManager->setRootFolder(folderPath);
        client->setWorkspaceManager(workspaceManager);
        if(workspaceManager->createWorkspaceFolders()){
            ui->consoleTextBrowser->append("<hr/>Рабочая папка организована!");
            //  создаем наблюдатель за папкой Entry
            ui->consoleTextBrowser->append(workspaceManager->setEntryWatcher());
            ui->consoleTextBrowser->append(workspaceManager->setProcessedWatcher());
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

//void MainWindow::SendPartOfFile()
//{
//    if((fileSize - file->pos()) < blockData){   //  если остаток файла будет меньше блока байт
//        blockData = fileSize - file->pos();     //  мы просто будем читать этот остаток
//    }

//    socket->waitForBytesWritten();  //  мы ждем того, чтобы все байты записались
//    Data.clear();

//    qDebug() << "read " << file->read(bytes, blockData);     //  читаем файл и записываем данные в байты
//    qDebug() << "block: "+QString::number(blockData);   //  нужно, чтобы видеть текущий размер блоков


//    QByteArray buffer;
//    buffer = buffer.fromRawData(bytes, blockData);

//    qDebug() << "block size" << blockData << "buffer size" << buffer.size();

//    QDataStream out(&Data, QIODevice::WriteOnly);   //  определяем поток отправки
//    out.setVersion(QDataStream::Qt_6_2);
//    out << quint64(0) << QString("File") << buffer;   //  отправляем байты
//    out.device()->seek(0);
//    //  избавляемся от зарезервированных двух байт в начале каждого сообщения
//    qDebug() << "sending blockSize = " << quint64(Data.size() - sizeof(quint64));
//    out << quint64(Data.size() - sizeof(quint64));   //  определяем размер сообщения
//    socket->write(Data);
//    qDebug() << "Data size = " << Data.size();
//}

//void MainWindow::SendToServer(QString typeOfMsg, QString str)   //  отдельная функция отправки сообщения, чтобы она не пересекалась с другими
//{
//    Data.clear();   //  может быть мусор

//    QDataStream out(&Data, QIODevice::WriteOnly);   //  объект out, режим работы только для записи, иначе ничего работать не будет
//    out.setVersion(QDataStream::Qt_6_2);
//    out << quint64(0) << typeOfMsg << str;  //  отправляем в поток размер_сообщения, тип-сообщения и строку при необходимости
//    out.device()->seek(0);  //  в начало потока
//    out << quint64(Data.size() - sizeof(quint64));  //  высчитываем размер сообщения
//    socket->write(Data);    //  отправляем данные на сервер
//}

//void MainWindow::slotReadyRead()
//{
//    ///     mapRequest[""] = "";  //  ничего не нужно
//    ///    mapRequest["001"] = "Message";   //  отправляется простое сообщение
//    ///    mapRequest["002"] = "File";  //  отправляется файл (определяем начало процесса передачи файла)
//    ///    mapRequest["102"] = "Request part of file";  //  запрос на еще одну часть файла
//    ///    mapRequest["012"] = "File downloaded";  //  файл загружен полностью (определяем конец процесса передачи файла)

//    QDataStream in(socket); //  создание объекта "in", помогающий работать с данными в сокете
//    in.setVersion(QDataStream::Qt_6_2);
//    if(in.status()==QDataStream::Ok){
//        while(true){    //  цикл для расчета размера блока
//            if(nextBlockSize == 0){ //  размер блока пока неизвестен
//                qDebug() << "nextBlockSize == 0";
//                if(socket->bytesAvailable() < 8){   //  и не должен быть меньше 8-и байт
//                    qDebug() << "Data < 8, break";
//                    break;  //  иначе выходим из цикла, т.е. размер посчитать невозможно
//                }
//                in >> nextBlockSize;    //  считываем размер блока в правильном исходе
//            }
//            if(socket->bytesAvailable() < nextBlockSize){   //  когда уже известен размер блока, мы сравниваем его с количеством байт, которые пришли от сервера
//                qDebug() << "Data not full | socket->bytesAvailable() = "+QString::number(socket->bytesAvailable()) + " | nextBlockSize = "+QString::number(nextBlockSize);    //  если данные пришли не полностью
//                break;
//            }
//            //  надо же, мы до сих пор в цикле, все хорошо

//            QString typeOfMessage;
//            in >> typeOfMessage;    //  определение типа сообщения

//            if(typeOfMessage == "Message"){ //  сервер прислал сообщение
//                QString str;    //  определяем переменную, в которую сохраним данные
//                in >> str;  //  выводим в переменную сообщение
//                qDebug() << str;
//                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str);   //  выводим полученное сообщение на экран
//            }

//            if(typeOfMessage == "File"){    //  отправляется файл

//                // mapRequest["002"] << fileName << fileSize

//                if(fileName.isEmpty()){    //  если файла не существует
//                    in >> fileName;  //  записываем из потока название файла
//                    in >> fileSize; //  считываем его размер

//                    if(fileSize < blockData){   //  если размер файла меньше выделенного блока
//                        blockData = fileSize;   //  устанавливаем размер блока ровно по файлу (передача произойдет в один этап)
//                    } else {
//                        blockData = 1000000;  //  устанавливаем по умолчанию (на случай последующей передачи, если размер файла будет куда больше)
//                    }

//                    file = new QFile;     //  определяем файл
//                    file->setFileName(fileName);    //  устанавливаем имя файла
//                    QDir::setCurrent(rawInformationDirectory);  //  устанавливаем путь сохранения на рабочем столе


//                    SendToServer(mapRequest["103"],"Downloading new part of processing file to "+socket->localAddress().toString()+"...");    //  запрашиваем первую часть файла
//                }
//            }

//            if(typeOfMessage == "Request part of file"){    //  если серверу нужна еще одна часть файла
//                QString str;    //  определяем переменную, в которую сохраним уведомление от запроса
//                in >> str;  //  выводим в переменную сообщение

//                qDebug() << str;  //  выводим в консоль
//                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str); //  выводим клиенту

//                nextBlockSize = 0;  //  заранее обнуляем размер сообщения
//                SendPartOfFile();   //  вызываем соответствующий метод отправки
//            }

//            if(typeOfMessage == "Request part of processing file"){
//                if((fileSize - file->size()) < blockData){  //  если разница между плановым и текущим размером файла меньше блока байтов
//                    blockData = fileSize - file->size();    //  мы устанавливаем такой размер для блока (разницу)
//                }

//                bytes = new char[blockData];   //  выделяем байты под файл, то есть передача пройдет в несколько этапов

//                in >> bytes;    //  считываем байты

//                if(file->open(QIODevice::Append)){  //  записываем в конец файла
//                    file->write(bytes, blockData);    //  записываем в файл
//                } else {
//                    qDebug() << "Не удается открыть файл "+fileName;
//                }

//                if(file->size() < fileSize){    //  если размер до сих пор не полон
//                    qDebug() << "Текущий размер файла "+fileName+" от "+QString::number(socket->socketDescriptor())+" = "+QString::number(file->size())+"\n"+"Ожидаемый размер = "+QString::number(fileSize);

//                    //  SendToAllClients(mapRequest["102"],"<font color = black><\\font>Downloading new part of file...<font color = black><\\font>");    //  запрашиваем новую часть файл
//                    SendToServer(mapRequest["103"],"<font color = black><\\font>Downloading new part of processing file...<font color = black><\\font>");    //  запрашиваем первую часть файла
//                } else {
//                    //  оформляем чат на стороне Сервера
//                    //  уведомление о "кто: какой файл" при сигнале "012" - File downloaded
//                    qDebug() << "Server: send file by name \""+fileName+"\"";
//                    //  SendToAllClients(mapRequest["012"],"<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

//                    SendToServer(mapRequest["012"], "<font color = green><\\font>file \""+fileName+"\" downloaded \n"+delimiter);

//                    //  TODO: при отправке всем происходит баг в задержке сообщений. решить
//                    //  SendToAllClients(mapRequest["001"], "<font color = green><\\font>User "+QString::number(socket->socketDescriptor())+" "+socket->localAddress().toString()+": send file by name \""+fileName+"\" \n"+delimiter);

//                    file->close();  //  закрываем файл
//                    file = nullptr; //  удаляем файл
//                    fileName.clear();   //  очищаем его название
//                    fileSize = 0;   //  очищаем его размер
//                    blockData = 1000000;  //  устанавливаем прежний размер байтов
//                    delete[] bytes; //  удаляем байты из кучи
//                    nextBlockSize = 0;  //  обнуляем для новых сообщений

//                    return; //  выходим
//                }

//                file->close();   //закрываем файл
//                if(bytes != nullptr){   //  удаляем байты из кучи, делая проверку на случай двойного удаления
//                    delete[] bytes;
//                    bytes = nullptr;
//                }
//                nextBlockSize = 0;  //  обнуляем для новых сообщений
//            }

//            if(typeOfMessage == "File downloaded"){ //  если файл полностью скачался
//                QString str;    //  определяем переменную, в которую сохраним данные
//                in >> str;  //  выводим в переменную сообщение
//                qDebug() << "File "+fileName+" downloaded";   //  выводим консоль, какой файл был загружен
//                ui->textBrowser->append(QTime::currentTime().toString()+" | "+str);  //  и то же самое клиенту
//                file->close();
//                delete file; //  удаляем файл
//                file = nullptr;
//                fileName.clear();   //  очищаем его название
//                delete[] bytes; //  удаляем байты из кучи
//                nextBlockSize = 0;  //  обнуляем для новых сообщений
//            }

//            if(typeOfMessage == "Possible treatments ComboBox data"){
//                possibleTreatments.clear();
//                ui->chooseTreatmentComboBox->clear();
//                in >> possibleTreatments;  //  выводим в глобальную переменную map из доступных обработок
//                qDebug() << possibleTreatments;

//                for(auto item = possibleTreatments.begin(); item != possibleTreatments.end(); ++item)
//                {
//                    //  Вставляем в комбобокс "в конец, читаемый текст, префикс"
//                    ui->chooseTreatmentComboBox->insertItem(ui->chooseTreatmentComboBox->count(), item.value(), item.key());
//                }
//                nextBlockSize = 0;  //  обнуляем для новых сообщений
//            }

//            if(typeOfMessage == "Disconnect"){
//                QString str;
//                in >> str;

//                qDebug() << "Disconnect";
//                ui->textBrowser->append(str);
//                setEnabledInterface(true);
//                socket->disconnectFromHost();
//            }

//            nextBlockSize = 0;  //  обнуляем для новых сообщений
//            if(socket->bytesAvailable() == 0){
//                break;  //  выходим, делать больше нечего
//            }
//        }   //  конец whil'a
//    } else {    //  если произошли проблемы с подключением
//        ui->textBrowser->append("Error connection");    //  уведомление клиента
//    }
//}

//void MainWindow::slotFolderForRawInformationChanged(const QString &folderName)
//{
//    QDir workWithDirectory;     //  создаем рабочую директорию
//    workWithDirectory.cd(folderName); //  переходим в нужный каталог

//    QStringList filters;    //  создаем список для фильтра
//    filters << "processed_*.txt";  //  заполняем

//    workWithDirectory.setNameFilters(filters);  //  устанавливаем фильтр
//    workWithDirectory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);     //  устанавливаем фильтр выводимых файлов/папок
//    workWithDirectory.setSorting(QDir::Size | QDir::Reversed);  //  устанавливаем сортировку "от меньшего к большему"
//    QFileInfoList list = workWithDirectory.entryInfoList();     //  получаем список файлов директории
//    for (int i = 0; i < list.size(); ++i) {
//        QFileInfo fileInfo = list.at(i);
//        qDebug() << qPrintable(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName()).arg(fileInfo.filePath()));   //  выводим в формате "размер имя"
//        SendFileToServer(fileInfo.filePath());  //  отправляем файл клиенту
//    }
//    qDebug() << folderName;
//    qDebug() << "================";     // переводим строку
//}

//void MainWindow::on_sendMsgPushButton_clicked() //  по нажатию на "Send msg"
//{
//    if(!ui->lineEdit->text().isEmpty()){    //  проверка на то, НЕ пустое ли сообщение
//        ui->filePathLabel->clear();     //  чистим от предыдущего уведомления
//        SendTextToServer("MESS:"+ui->lineEdit->text());     //  отправляем сообщение со служебным префиксом
//    } else {
//        ui->filePathLabel->setText("Your msg is empty!");   //  уведомляем о пустом сообщении
//    }
//}

//void MainWindow::on_chooseTreatmentPushButton_clicked() //  по нажатию на кнопку "Choose treatment"
//{
//    SendToServer(mapRequest["0041"], ui->chooseTreatmentComboBox->currentData().toString());   //  отправляем серверу текущий текст в комбобоксе
//    ui->textBrowser->append("Selected "+ui->chooseTreatmentComboBox->currentData().toString()+delimiter);   //  пишем клиенту, что он выбрал
//}

