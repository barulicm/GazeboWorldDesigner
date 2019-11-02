#include <QtWidgets/QFileDialog>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QMessageBox>
#include <sdf/Parser.h>
#include <QtCore/QTextStream>
#include "FileManager.h"

FileManager::FileManager(QObject *parent) : QObject(parent) {

}

World &FileManager::getWorld() {
  return world;
}

const QString &FileManager::getCurrentPath() const {
  return currentFilePath;
}

void FileManager::newFile() {
  world = World{};
  currentFilePath.clear();
  emit onNewWorld(world);
  emit onCurrentPathChanged(currentFilePath);
}

void FileManager::openFile() {
  auto parentWidget = dynamic_cast<QWidget*>(parent());
  auto filename = QFileDialog::getOpenFileName(parentWidget, "Open World",
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "SDF Files (*.sdf *.world *.xml");

  if(filename.isEmpty()) {
    return;
  }

  QFile file{filename};
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(parentWidget, "Gazebo World Designer", "Could not open world file\n" + filename, QMessageBox::Ok);
    return;
  }

  QDomDocument document;
  document.setContent(file.readAll());
  file.close();

  world = Parser::ParseWorld(document);

  currentFilePath = filename;

  emit onNewWorld(world);
  emit onCurrentPathChanged(currentFilePath);
}

void FileManager::saveFile() {
  if(currentFilePath.isEmpty()) {
    saveFileAs();
  } else {
    writeWorldToFile(currentFilePath);
  }
}

void FileManager::saveFileAs() {
  auto parentWidget = dynamic_cast<QWidget*>(parent());
  auto newFileName = QFileDialog::getSaveFileName(parentWidget, "Save World", QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "SDF Files (*.sdf *.world *.xml");
  if(newFileName.isEmpty()) {
    return;
  }
  currentFilePath = newFileName;
  writeWorldToFile(currentFilePath);
  emit onCurrentPathChanged(currentFilePath);
}

void FileManager::writeWorldToFile(const QString &filepath) {
  auto parentWidget = dynamic_cast<QWidget*>(parent());

  QDomDocument xmlDocument;
  auto xml = world.toXML(xmlDocument);

  QDomElement sdfElement = xmlDocument.createElement("sdf");
  sdfElement.setAttribute("version", "1.4");
  sdfElement.appendChild(xml);
  xmlDocument.appendChild(sdfElement);

  QFile file{filepath};
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QMessageBox::critical(parentWidget, "Gazebo World Designer",
                          ("Unable to open file for saving: " + currentFilePath), QMessageBox::Ok);
    return;
  }
  QTextStream textStream{&file};
  xmlDocument.save(textStream,1);
  textStream.flush();
  file.close();
}
