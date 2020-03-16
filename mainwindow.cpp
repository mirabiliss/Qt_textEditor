#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(createContextMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton res = QMessageBox::question(this, "Exit", tr("Do you want to discard all changes?"), \
                                                   QMessageBox::Discard | QMessageBox::Cancel | QMessageBox::Save,\
                                                   QMessageBox::Save);
    if(res == QMessageBox::Cancel){
        event->ignore();
    }
    else if (res == QMessageBox::Save){
        on_actionSave_triggered();
        event->accept();
    }
    else{
        event->accept();
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (!isFileSaved){
        QMessageBox *box = new QMessageBox(this);
        box->setWindowTitle("Alert");
        box->setText("Do you want to discard all changes?");
        box->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box->setDefaultButton(QMessageBox::Save);

        auto res = box->exec();
        if (res == QMessageBox::Save){
            on_actionSave_triggered();
            ui->textEdit->clear();
        }
        else if (res == QMessageBox::Cancel){
            box->close();
        }
        else{
            ui->textEdit->clear();
        }
    }
    currOpenedFilePath = "untitled";
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog* dialog = new QFileDialog(this);
    QFile currFile(dialog->getOpenFileName());
    currFile.open(QIODevice::ReadOnly);

    if (currFile.isOpen()){
        ui->textEdit->setText(currFile.readAll());
        currFile.close();
    }
    currOpenedFilePath = currFile.fileName();
}

void MainWindow::on_actionSave_triggered()
{
    if(currOpenedFilePath == "untitled"){
        on_actionSave_as_triggered();
    }
    else{
        QFile file(currOpenedFilePath);
        file.open(QIODevice::WriteOnly);
        file.write(ui->textEdit->toPlainText().toUtf8());
        file.close();
    }
    this->isFileSaved = true;
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog* dialog = new QFileDialog(this);
    QString new_path = dialog->getSaveFileName(this);

    QFile file(new_path);
    file.open(QIODevice::WriteOnly);
    file.write(ui->textEdit->toPlainText().toUtf8());
    file.close();

    this->currOpenedFilePath = new_path;
    this->isFileSaved = true;
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionFind_triggered()
{
    QString toFind = QInputDialog::getText(this, tr("Find"), tr("Keyword: "), \
                                          QLineEdit::Normal);
    QTextCursor c(ui->textEdit->document());
    c = ui->textEdit->document()->find(toFind, c);
    int startPos = c.selectionStart();
    int endPos = c.selectionStart() + toFind.length();
    //while (ui->textEdit->toPlainText()[endPos].isLetter()) endPos++;

    c.setPosition(startPos);
    c.setPosition(endPos, QTextCursor::KeepAnchor);
    ui->textEdit->setTextCursor(c);
}

void MainWindow::on_actionReplace_triggered()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Replace v.1 with v.2: "));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;
    for(int i = 0; i < 2; ++i) {
        QLineEdit *lineEdit = new QLineEdit(&dialog);
        QString label = QString("Value %1").arg(i + 1);
        form.addRow(label, lineEdit);

        fields << lineEdit;
    }

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    QString toReplace, repl;

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        toReplace = fields[0]->text();
        repl = fields[1]->text();
    }

    QTextCursor c(ui->textEdit->document());
    c = ui->textEdit->document()->find(toReplace, c);
    int startPos = c.selectionStart();
    int endPos = c.selectionStart() + toReplace.length();
    //while (ui->textEdit->toPlainText()[endPos].isLetter()) endPos++;

    c.setPosition(startPos);
    c.setPosition(endPos, QTextCursor::KeepAnchor);
    ui->textEdit->setTextCursor(c);

    ui->textEdit->insertPlainText(repl);
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionExit_triggered()
{
    if (!isFileSaved){
        QMessageBox *box = new QMessageBox(this);
        box->setWindowTitle("Exit");
        box->setText("Do you want to discard all changes?");
        box->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box->setDefaultButton(QMessageBox::Save);

        auto res = box->exec();
        if (res == QMessageBox::Save){
            on_actionSave_triggered();

            QApplication::quit();
        }
        else if (res == QMessageBox::Cancel){
            box->close();
        }
        else{
            QApplication::quit();
        }
    }
}

void MainWindow::on_textEdit_textChanged()
{
    this->isFileSaved = false;
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times New Roman", 12), this);
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();

    format.setFont(font);
    cursor.setCharFormat(format);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrintDialog* printDialog = new QPrintDialog(this);
    printDialog->exec();
}

void MainWindow::on_actionColor_triggered()
{
    QColorDialog* dialog = new QColorDialog(this);
    QColor color;
    ui->textEdit->setTextColor(dialog->getColor(color, this, "Choose color: "));
}


void MainWindow::createActions(){
    newAction = new QAction("New", this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(on_actionNew_triggered()));

    openAction = new QAction("Open...", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(on_actionOpen_triggered()));

    saveAction = new QAction("Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));

    saveAsAction = new QAction("Save as...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(on_actionSave_as_triggered()));

    copyAction = new QAction("Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(on_actionCopy_triggered()));

    pasteAction = new QAction("Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(on_actionPaste_triggered()));

    findAction = new QAction("Find...", this);
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, SIGNAL(triggered()), this, SLOT(on_actionFind_triggered()));

    replaceAction = new QAction("Replace...", this);
    replaceAction->setShortcut(QKeySequence::Replace);
    connect(replaceAction, SIGNAL(triggered()), this, SLOT(on_actionReplace_triggered()));

    undoAction = new QAction("Undo", this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(on_actionUndo_triggered()));

    redoAction = new QAction("Redo", this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, SIGNAL(triggered()), this, SLOT(on_actionRedo_triggered()));

    colorAction = new QAction("Color", this);
    colorAction->setShortcut(QKeySequence::fromString("Ctrl + F7"));
    connect(colorAction, SIGNAL(triggered()), this, SLOT(on_actionColor_triggered()));

    fontAction = new QAction("Font", this);
    fontAction->setShortcut(QKeySequence::fromString("Ctrl + F8"));
    connect(fontAction, SIGNAL(triggered()), this, SLOT(on_actionFont_triggered()));

    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence::fromString("Ctrl + F10"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(on_actionExit_triggered()));


}


void MainWindow::createContextMenu(const QPoint& pos){
    editMenu->addAction(newAction);
    editMenu->addAction(openAction);
    editMenu->addAction(saveAction);
    editMenu->addAction(saveAsAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(findAction);
    editMenu->addAction(replaceAction);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(exitAction);
    editMenu->addAction(fontAction);
    editMenu->addAction(colorAction);
    editMenu->addAction(printAction);

    editMenu->exec(mapToGlobal(pos));
}
