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
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.cpp \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.cpp \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.cpp \
    components/frames/cardFrame/changePortLineEditFrame/change_port_line_edit_frame.cpp \
    helperClasses/jsonPacker/json_packer.cpp \
    helperClasses/jsonParser/json_parser.cpp \
    helperClasses/managers/processingManager/processing_manager.cpp \
    helperClasses/managers/readyReadManager/ready_read_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsRequestPartProcessingFile/clients_request_part_processing_file.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsMessageManager/clients_message_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsProcessingManager/clients_processing_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsFileManager/clients_file_manager.cpp \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.cpp \
    helperClasses/managers/workspaceManager/settingsManager/settings_manager.cpp \
    helperClasses/managers/workspaceManager/workspace_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    components/dialogs/I_dialogwindow.h \
    components/dialogs/updateProcessingDialog/update_processing_dialog.h \
    components/frames/cardFrame/I_cardframe.h \
    components/frames/cardFrame/maxConnectionSpinBoxFrame/max_connection_spinbox_frame.h \
    components/frames/cardFrame/selectWorkspaceFrame/select_workspace_frame.h \
    components/frames/cardFrame/possibleProcessingComboBoxFrame/possible_processing_combobox_frame.h \
    components/frames/cardFrame/changePortLineEditFrame/change_port_line_edit_frame.h \
    helperClasses/jsonPacker/json_packer.h \
    helperClasses/jsonParser/json_parser.h \
    helperClasses/managers/processingManager/processing_manager.h \
    helperClasses/managers/readyReadManager/ready_read_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/I_message_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsRequestPartProcessingFile/clients_request_part_processing_file.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsMessageManager/clients_message_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsProcessingManager/clients_processing_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/clientsFileManager/clients_file_manager.h \
    helperClasses/managers/readyReadManager/supportRRManagers/nullManager/null_manager.h \
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
