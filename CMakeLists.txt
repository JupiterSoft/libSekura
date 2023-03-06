cmake_minimum_required(VERSION 3.14)

project(libSekura LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network Core5Compat Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network
                                                       Core5Compat Gui)

if(APPLE)
  set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl")
endif()

find_package(OpenSSL)

message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
include_directories(${OPENSSL_INCLUDE_DIR})

add_library(
  libSekura STATIC
  sekura.cpp
  sekura.h
  Sekura
  restclient.h
  restclient.cpp
  RestClient
  restsettings.h
  restsettings.cpp
  RestSettings
  authdialog.h
  authdialog.cpp
  authdialog.ui
  AuthDialog
  tablemodel.h
  tablemodel.cpp
  TableModel
  Sekura
  baseitem.h
  baseitem.cpp
  BaseItem
  lineedit.h
  lineedit.cpp
  lineedit.ui
  LineEdit
  datetimeedit.h
  datetimeedit.cpp
  datetimeedit.ui
  DateTimeEdit
  combobox.h
  combobox.cpp
  combobox.ui
  ComboBox
  spinbox.h
  spinbox.cpp
  spinbox.ui
  SpinBox
  tablewidget.h
  tablewidget.cpp
  tablewidget.ui
  TableWidget
  itemwidget.h
  itemwidget.cpp
  itemwidget.ui
  ItemWidget
  checkbox.h
  checkbox.cpp
  checkbox.ui
  CheckBox
  bottombuttonswidget.h
  bottombuttonswidget.cpp
  bottombuttonswidget.ui
  BottomButtonsWidget
  itemmodel.h
  itemmodel.cpp
  ItemModel
  treemodel.h
  treemodel.cpp
  TreeModel
  treewidget.h
  treewidget.cpp
  treewidget.ui
  TreeWidget
  menu.h
  menu.cpp
  Menu
  basewidget.h
  basewidget.cpp
  BaseWidget
  bottomtablewidget.h
  bottomtablewidget.cpp
  bottomtablewidget.ui
  BottomTableWidget
  modelfilter.h
  modelfilter.cpp
  ModelFilter
  wizarditemwidget.h
  wizarditemwidget.cpp
  wizarditemwidget.ui
  WizardItemWidget
  datatype.h
  datatype.cpp
  datatype.ui
  DataType
  foreignkey.h
  foreignkey.cpp
  foreignkey.ui
  ForeignKey
  element.h
  element.cpp
  element.ui
  Element
  qaesencryption.h
  qaesencryption.cpp
  QAESEncryption
  listmodel.h
  listmodel.cpp
  ListModel
  listwidget.h
  listwidget.cpp
  listwidget.ui
  ListWidget
  text.h
  text.cpp
  text.ui
  Text
  mainwindow.h
  mainwindow.cpp
  mainwindow.ui
  MainWindow
  appobject.h
  appobject.cpp
  resources.qrc)

target_link_libraries(
  libSekura
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
          Qt${QT_VERSION_MAJOR}::Core5Compat Qt${QT_VERSION_MAJOR}::Gui ${OPENSSL_LIBRARIES})

target_compile_definitions(libSekura PRIVATE LIBSEKURA_LIBRARY)
