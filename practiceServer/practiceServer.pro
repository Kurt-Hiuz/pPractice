QT       += core gui
QT += network    #   network - используется для работы с сетевыми классами

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/dialogs/I_dialogwindow.cpp \
    components/dialogs/updateProcessingDialog/update_processing_dialog.cpp \
    components/frames/cardFrame/I_cardframe.cpp \
    components/frames/cardFrame/changePortSpinBoxFrame/change_port_spinbox_frame.cpp \
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.cpp \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.cpp \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.cpp \
    helperClasses/jsonPacker/json_packer.cpp \
    helperClasses/jsonParser/json_parser.cpp \
    helperClasses/managers/readyReadManager/ready_read_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/serverRequestPartFileManager/server_request_part_file_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsMessageManager/clients_message_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsProcessingManager/clients_processing_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsFileManager/clients_file_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/fileDownloadedManager/file_downloaded_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.cpp \
    helperClasses/managers/workspaceManager/storageManager/storage_manager.cpp \
    helperClasses/managers/workspaceManager/entryManager/entry_manager.cpp \
    helperClasses/managers/workspaceManager/expectationManager/expectation_manager.cpp \
    helperClasses/managers/workspaceManager/sendedFileManager/sended_file_manager.cpp \
    helperClasses/managers/workspaceManager/settingsManager/settings_manager.cpp \
    helperClasses/managers/workspaceManager/workspace_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    components/dialogs/I_dialogwindow.h \
    components/dialogs/updateProcessingDialog/update_processing_dialog.h \
    components/frames/cardFrame/I_cardframe.h \
    components/frames/cardFrame/changePortSpinBoxFrame/change_port_spinbox_frame.h \
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.h \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.h \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.h \
    helperClasses/jsonPacker/json_packer.h \
    helperClasses/jsonParser/json_parser.h \
    helperClasses/managers/readyReadManager/ready_read_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/serverRequestPartFileManager/server_request_part_file_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsMessageManager/clients_message_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsProcessingManager/clients_processing_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsFileManager/clients_file_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/fileDownloadedManager/file_downloaded_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.h \
    helperClasses/managers/workspaceManager/storageManager/storage_manager.h \
    helperClasses/managers/workspaceManager/entryManager/entry_manager.h \
    helperClasses/managers/workspaceManager/expectationManager/expectation_manager.h \
    helperClasses/managers/workspaceManager/sendedFileManager/sended_file_manager.h \
    helperClasses/managers/workspaceManager/settingsManager/settings_manager.h \
    helperClasses/managers/workspaceManager/workspace_manager.h \
    mainwindow.h \
    server.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
