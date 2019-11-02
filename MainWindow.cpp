#include <QMenu>
#include <QMenuBar>
#include <QHeaderView>
#include <QSplitter>
#include <QTreeView>
#include "ModelTreeModel.h"
#include "PropertyPane.h"
#include "MainWindow.h"

MainWindow::MainWindow() {
    setWindowTitle("Gazebo World Designer");

    auto fileManager = new FileManager(this);

    auto worldView = new WorldView;

    createMenus(fileManager);

    auto splitter = new QSplitter{this};
    splitter->addWidget(createLeftPanel());
    splitter->addWidget(worldView);
    splitter->addWidget(createRightPanel(fileManager, worldView));

    setCentralWidget(splitter);

    connect(fileManager, &FileManager::onCurrentPathChanged, [this](const QString &path){
      if(path.isEmpty()) {
        setWindowTitle("Gazebo World Designer");
      } else {
        setWindowTitle("Gazebo World Designer - " + path);
      }
    });

    connect(fileManager, &FileManager::onNewWorld, worldView, &WorldView::newWorld);

    fileManager->newFile();
}

void MainWindow::createMenus(FileManager *fileManager) {
    auto closeAct = new QAction{"&Close", this};
    closeAct->setShortcuts(QKeySequence::Close);
    connect(closeAct, &QAction::triggered, this, &QMainWindow::close);
    addAction(closeAct);

    auto newFileAct = new QAction{"&New", this};
    newFileAct->setShortcuts(QKeySequence::New);
    connect(newFileAct, &QAction::triggered, fileManager, &FileManager::newFile);
    addAction(newFileAct);

    auto openFileAct = new QAction{"&Open", this};
    openFileAct->setShortcuts(QKeySequence::Open);
    connect(openFileAct, &QAction::triggered, fileManager, &FileManager::openFile);
    addAction(openFileAct);

    auto saveFileAct = new QAction{"&Save", this};
    saveFileAct->setShortcuts(QKeySequence::Save);
    connect(saveFileAct, &QAction::triggered, fileManager, &FileManager::saveFile);
    addAction(saveFileAct);

    auto saveFileAsAct = new QAction{"&Save As", this};
    saveFileAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveFileAsAct, &QAction::triggered, fileManager, &FileManager::saveFileAs);
    addAction(saveFileAsAct);

    auto fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveFileAct);
    fileMenu->addAction(saveFileAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
}

QWidget *MainWindow::createLeftPanel() {
  auto leftSideLayout = new QVBoxLayout;
  leftSideLayout->addWidget(new QLabel{"Available Models"});

  auto treeModel = new ModelTreeModel;
  auto modelTreeView = new QTreeView;
  modelTreeView->setModel(treeModel);
  modelTreeView->header()->hide();
  modelTreeView->setDragEnabled(true);
  modelTreeView->viewport()->setAcceptDrops(false);
  modelTreeView->setDragDropMode(QAbstractItemView::DragOnly);
  leftSideLayout->addWidget(modelTreeView);

  auto leftLine = new QFrame();
  leftLine->setFrameShape(QFrame::HLine);
  leftLine->setFrameShadow(QFrame::Plain);
  leftSideLayout->addWidget(leftLine);

  leftSideLayout->addWidget(new QLabel{"Current World"});

  auto leftSideWidget = new QWidget;
  leftSideWidget->setLayout(leftSideLayout);

  return leftSideWidget;
}

QWidget *MainWindow::createRightPanel(FileManager *fileManager,
                                      WorldView *worldView) {
  auto propertyPanesLayout = new QVBoxLayout;
  propertyPanesLayout->addWidget(new QLabel{"Object Properties"});
  auto objectPropertyPane = new PropertyPane;
  propertyPanesLayout->addWidget(objectPropertyPane);
  propertyPanesLayout->addStretch();
  auto line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Plain);
  propertyPanesLayout->addWidget(line);
  propertyPanesLayout->addWidget(new QLabel{"Scene Properties"});
  auto scenePropertyPane = new PropertyPane;
  propertyPanesLayout->addWidget(scenePropertyPane);

  connect(fileManager, &FileManager::onNewWorld, [scenePropertyPane](World &world){
      scenePropertyPane->showProperties(&world.scene);
  });

  connect(worldView, &WorldView::onSelectedIndexChanged, [objectPropertyPane,world=&fileManager->getWorld()](int index){
      if(index >= 0) {
          objectPropertyPane->showProperties(world->elements[static_cast<size_t>(index)].get());
      } else {
          objectPropertyPane->clear();
      }
  });

  auto propertyPanesWidget = new QWidget;
  propertyPanesWidget->setLayout(propertyPanesLayout);

  return propertyPanesWidget;
}
