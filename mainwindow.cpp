/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <QtWidgets>

#include "mainwindow.h"
//! [0]

//! [1]
MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit)
//! [1] //! [2]
{
    setCentralWidget(textEdit);

    createActions();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [2]

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {//TODO: maybe make new doc
        textEdit->clear();
        //TODO: select base type
        setCurrentFile(QString());
    }
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QFileDialog dialog(this, tr("Open File"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);

        QStringList mimeTypes;
        mimeTypes << "text/plain";
        dialog.setMimeTypeFilters(mimeTypes);
        if (dialog.exec() != QDialog::Accepted)
            return;
        //TODO: maybe need to do view transform on type
        if (!dialog.selectedFiles().first().isEmpty())
            loadFile(dialog.selectedFiles().first());
    }
}
//! [8]

//! [9]
void MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        saveAs();
    } else {
        saveFile(curFile);
    }
}
//! [10]

void MainWindow::publish() {
    maybeSave();
    QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss");
    if(QProcess::execute("git add . && git stash && git pull && git stash pop" //incorporate
        "git commit -m \"" + now + "\" && git push") != 0) {
        QMessageBox::critical(this, tr("Publication Error"),
                 tr("The repository could not be published."));
        return;
    }
    statusBar()->showMessage(tr("Published"), 2000);
}

void MainWindow::subscribe() {
    if(QProcess::execute("git add . && git stash && git pull && git stash pop") != 0) { //incorporate
        QMessageBox::critical(this, tr("Reading Error"),
                 tr("The repository could not be read."));
        return;
    }
    statusBar()->showMessage(tr("Read"), 2000);
}

//! [11]
void MainWindow::saveAs()
//! [11] //! [12]
{
    QFileDialog dialog(this, tr("Save File"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList mimeTypes;
    mimeTypes << "text/plain";
    dialog.setMimeTypeFilters(mimeTypes);
    if (dialog.exec() != QDialog::Accepted)
        return;
    //TODO: maybe need to do view transform on type
    saveFile(dialog.selectedFiles().first());
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
   QMessageBox::about(this, tr("About QtAp"),
            tr("<b>QtAp</b> for document control. "
               "Writen in C++ using Qt. "));
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    setWindowModified(textEdit->document()->isModified());
}
//! [16]
QMenu* MainWindow::addMenu(QString menu, void(MainWindow::*fp)(),
                         QString named, QString entry,
                         QKeySequence::StandardKey shorty,
                         QString help, bool noBar) {
    static QMenu *aMenu;
    static QToolBar *aToolBar;
    if(menu != nullptr) {
        aMenu = menuBar()->addMenu(menu);
        aToolBar = addToolBar(menu.replace('&', ""));
    }
    if(aMenu == nullptr) {
        aMenu = menuBar()->addMenu("&Menu");
        aToolBar = addToolBar("Menu");
    }

    const QIcon newIcon = QIcon::fromTheme(named, QIcon(":/images/" + named + ".png"));
    QAction *newAct = new QAction(newIcon, entry, this);
    newAct->setShortcuts(shorty);
    newAct->setStatusTip(help);
    connect(newAct, &QAction::triggered, this, fp);
    aMenu->addAction(newAct);
    if(noBar) return aMenu;
    aToolBar->addAction(newAct);
    return aMenu;
}
void MainWindow::close() {
    QWidget::close();
}
//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
    addMenu(tr("&File"), &MainWindow::newFile,
            "document-new", tr("&New"), QKeySequence::New,
            tr("Create a new file"));
    addMenu(nullptr, &MainWindow::open,
            "document-open", tr("&Open..."), QKeySequence::Open,
            tr("Open an existing file"));
    addMenu(nullptr, &MainWindow::save,
            "document-save", tr("&Save"), QKeySequence::Save,
            tr("Save the document to disk"));
    addMenu(nullptr, &MainWindow::saveAs,
            "document-save-as", tr("Save &As..."), QKeySequence::SaveAs,
            tr("Save the document under a new name"), true)->addSeparator();//no bar entry
    addMenu(nullptr, &MainWindow::close,
            "application-exit", tr("E&xit"), QKeySequence::Quit,
            tr("Exit the application"), true);//no bar entry

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/edit-cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);

    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/edit-copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/edit-paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QToolBar *viewToolBar = addToolBar(tr("View"));

    const QIcon viewTextIcon = QIcon::fromTheme("view-text", QIcon(":/images/view-text.png"));
    QAction *viewAct = new QAction(viewTextIcon, tr("&Text"), this);
    viewAct->setShortcuts(QKeySequence::AddTab);
    viewAct->setStatusTip(tr("Show editable text view"));//the base view

    // TODO: No menu

    viewMenu->addAction(viewAct);
    viewToolBar->addAction(viewAct);

    menuBar()->addSeparator();

    QMenu *syncMenu = menuBar()->addMenu(tr("&Sync"));
    QToolBar *syncToolBar = addToolBar(tr("Sync"));

    const QIcon syncIcon = QIcon::fromTheme("sync-publish", QIcon(":/images/sync-publish.png"));
    QAction *syncAct = new QAction(syncIcon, tr("&Publish"), this);
    syncAct->setShortcuts(QKeySequence::Print);
    syncAct->setStatusTip(tr("Publish with services"));//the main action
    connect(syncAct, &QAction::triggered, this, &MainWindow::publish);
    syncMenu->addAction(syncAct);
    syncToolBar->addAction(syncAct);

    const QIcon readIcon = QIcon::fromTheme("sync-read", QIcon(":/images/sync-read.png"));
    QAction *readAct = new QAction(readIcon, tr("&Read"), this);
    readAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    readAct->setStatusTip(tr("Read from services"));//the main action
    connect(readAct, &QAction::triggered, this, &MainWindow::subscribe);
    syncMenu->addAction(readAct);
    syncToolBar->addAction(readAct);

    // TODO: No menu

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

//! [22]

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//! [22]

//! [23]
#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}
//! [24]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Save Changes"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        save();
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";//default to text
    //TODO: set default as note taking
    setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}
//! [49]
#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif
