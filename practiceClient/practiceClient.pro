QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    components/frames/cardFrame/I_cardframe.cpp \
    components/frames/cardFrame/mainTabFrames/chatFrame/chat_frame.cpp \
    components/frames/cardFrame/mainTabFrames/connectFrame/connect_frame.cpp \
    components/frames/cardFrame/mainTabFrames/fileFrame/file_frame.cpp \
    components/frames/cardFrame/mainTabFrames/possibleProcessingFrame/possible_processing_frame.cpp \
    components/frames/cardFrame/settingsTabFrames/selectWorkspaceFrame/select_workspace_frame.cpp \
    helperClasses/managers/processingManager/processing_manager.cpp \
    helperClasses/managers/readyReadManager/ready_read_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientFileRequestPartManager/client_file_request_part_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/disconnectManager/disconnect_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/fileDownloadedManager/file_downloaded_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/possibleProcessingManager/possible_processing_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/serverFileManager/server_file_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/serverMessageManager/server_message_manager.cpp \
    helperClasses/managers/workspaceManager/entryManager/entry_manager.cpp \
    helperClasses/managers/workspaceManager/processedManager/processed_manager.cpp \
    helperClasses/managers/workspaceManager/workspace_manager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    client.h \
    components/frames/cardFrame/I_cardframe.h \
    components/frames/cardFrame/mainTabFrames/chatFrame/chat_frame.h \
    components/frames/cardFrame/mainTabFrames/connectFrame/connect_frame.h \
    components/frames/cardFrame/mainTabFrames/fileFrame/file_frame.h \
    components/frames/cardFrame/mainTabFrames/possibleProcessingFrame/possible_processing_frame.h \
    components/frames/cardFrame/settingsTabFrames/selectWorkspaceFrame/select_workspace_frame.h \
    helperClasses/managers/processingManager/processing_manager.h \
    helperClasses/managers/readyReadManager/ready_read_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientFileRequestPartManager/client_file_request_part_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/disconnectManager/disconnect_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/fileDownloadedManager/file_downloaded_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/possibleProcessingManager/possible_processing_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/serverFileManager/server_file_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/serverMessageManager/server_message_manager.h \
    helperClasses/managers/workspaceManager/entryManager/entry_manager.h \
    helperClasses/managers/workspaceManager/processedManager/processed_manager.h \
    helperClasses/managers/workspaceManager/workspace_manager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
