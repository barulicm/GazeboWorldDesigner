#ifndef GWD_FILEMANAGER_H
#define GWD_FILEMANAGER_H

#include <QObject>
#include <QtXml/QDomDocument>
#include <sdf/World.h>

class FileManager : public QObject {
  Q_OBJECT
public:
  explicit FileManager(QObject *parent = nullptr);

  World& getWorld();
  const QString &getCurrentPath() const;

signals:
  void onNewWorld(World &world);
  void onCurrentPathChanged(const QString &path);

public slots:
  void newFile();
  void openFile();
  void saveFile();
  void saveFileAs();


private:
  QString currentFilePath;
  World world;

  void writeWorldToFile(const QString &filepath);

};


#endif //GWD_FILEMANAGER_H
