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

    createMenus();

    auto splitter = new QSplitter{this};
    splitter->addWidget(createLeftPanel());
    splitter->addWidget(&worldView);
    splitter->addWidget(createRightPanel());

    setCentralWidget(splitter);

    worldView.newFile();
}

void MainWindow::createMenus() {
    auto closeAct = new QAction{"&Close", this};
    closeAct->setShortcuts(QKeySequence::Close);
    connect(closeAct, &QAction::triggered, this, &QMainWindow::close);
    addAction(closeAct);

    auto newFileAct = new QAction{"&New", this};
    newFileAct->setShortcuts(QKeySequence::New);
    connect(newFileAct, &QAction::triggered, &worldView, &WorldView::newFile);
    addAction(newFileAct);

    auto openFileAct = new QAction{"&Open", this};
    openFileAct->setShortcuts(QKeySequence::Open);
    connect(openFileAct, &QAction::triggered, &worldView, &WorldView::openFile);
    addAction(openFileAct);

    auto saveFileAct = new QAction{"&Save", this};
    saveFileAct->setShortcuts(QKeySequence::Save);
    connect(saveFileAct, &QAction::triggered, &worldView, &WorldView::saveFile);
    addAction(saveFileAct);

    auto saveFileAsAct = new QAction{"&Save As", this};
    saveFileAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveFileAsAct, &QAction::triggered, &worldView, &WorldView::saveFileAs);
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

QWidget *MainWindow::createRightPanel() {
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

  connect(&worldView, &WorldView::showProperties, objectPropertyPane, &PropertyPane::showProperties);
  connect(&worldView, &WorldView::showSceneProperties, scenePropertyPane, &PropertyPane::showProperties);

  auto propertyPanesWidget = new QWidget;
  propertyPanesWidget->setLayout(propertyPanesLayout);

  return propertyPanesWidget;
}
