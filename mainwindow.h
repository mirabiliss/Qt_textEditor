#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFontDialog>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextDocumentFragment>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QColorDialog>
#include <QMenu>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent (QCloseEvent *event) override;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *findAction;
    QAction *replaceAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *exitAction;
    QAction *fontAction;
    QAction *printAction;
    QAction *colorAction;
    QMenu *editMenu = new QMenu(this);

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_as_triggered();
    void on_actionSave_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionExit_triggered();
    void on_textEdit_textChanged();
    void on_actionFont_triggered();
    void on_actionPrint_triggered();
    void on_actionColor_triggered();
    void createActions();
    void createContextMenu(const QPoint& pos);

private:
    Ui::MainWindow *ui;
    QString currOpenedFilePath = "untitled";
    bool isFileSaved = true;
};

#endif // MAINWINDOW_H
