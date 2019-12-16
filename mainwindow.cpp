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

#include <QtWidgets>
#include "mainwindow.h"

constexpr Spec operator|(Spec X, Spec Y) {
    return static_cast<Spec>(
        static_cast<unsigned int>(X) | static_cast<unsigned int>(Y));
}

Spec& operator|=(Spec& X, Spec Y) {
    X = X | Y; return X;
}

MainWindow::MainWindow()
    : textEdit(new QPlainTextEdit) {
    setWindowIcon(getIconRC("view-text"));
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

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile() {
    if (maybeSave()) {//TODO: maybe make new doc
        textEdit->clear();
        //TODO: select base type
        setCurrentFile(QString());
    }
}

void MainWindow::setRepo() {
    QFileDialog dialog(this, tr("Set Sync"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setNameFilterDetailsVisible(false);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setLabelText(QFileDialog::Accept, tr("Set"));

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (!dialog.selectedFiles().first().isEmpty())
        directory = dialog.selectedFiles().first();
}

void MainWindow::open() {
    if (maybeSave()) {
        QFileDialog dialog(this, tr("Open File"), directory);
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

void MainWindow::save() {
    if (curFile.isEmpty()) {
        saveAs();
    } else {
        saveFile(curFile);
    }
}

void MainWindow::publish() {
    maybeSave();
    QString now = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss");
    if(QProcess::execute("cd " + directory +"&&"
        "git add .&&git stash&&git pull&&git stash pop&&" //incorporate
        "git commit -m \"" + now + "\"&&git push") != 0) {
        QMessageBox::critical(this, tr("Publication Error"),
                 tr("The repository could not be published."));
        return;
    }
    statusBar()->showMessage(tr("Published"), 2000);
}

void MainWindow::subscribe() {
    if(QProcess::execute("cd " + directory +"&&"
        "git add .&&git stash&&git pull&&git stash pop") != 0) { //incorporate
        QMessageBox::critical(this, tr("Reading Error"),
                 tr("The repository could not be read."));
        return;
    }
    statusBar()->showMessage(tr("Read"), 2000);
}

void MainWindow::saveAs() {
    QFileDialog dialog(this, tr("Save File"), directory);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList mimeTypes;
    mimeTypes << "text/plain";
    dialog.setMimeTypeFilters(mimeTypes);
    saved = true;
    if (dialog.exec() != QDialog::Accepted) {
        saved = false;
        return;
    }
    //TODO: maybe need to do view transform on type
    saveFile(dialog.selectedFiles().first());
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QtAp"),
            tr("<b>QtAp</b> for document control. "
               "Writen in C++ using Qt. "
               "It requires Git for sync to work. "));
}

void MainWindow::documentWasModified() {
    setWindowModified(textEdit->document()->isModified());
}

QIcon MainWindow::getIconRC(QString named) {
    return QIcon::fromTheme(named, QIcon(":/images/" + named + ".png"));
}

QMenu* MainWindow::addMenu(QString menu, void(MainWindow::*fp)(),
                         QString named, QString entry,
                         QKeySequence shorty,
                         QString help, Spec option) {
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

    if(named == nullptr) {
        named = QString("void");
        option |= noBar;//as sensible
    }
    const QIcon newIcon = getIconRC(named);
    QAction *newAct = new QAction(newIcon, entry, this);
    if(shorty > 0) newAct->setShortcut(shorty);
    if(help != nullptr) newAct->setStatusTip(help);
    connect(newAct, &QAction::triggered, this, fp);
    aMenu->addAction(newAct);
    if(option & canCopy) {
        newAct->setEnabled(false);
        connect(textEdit, &QPlainTextEdit::copyAvailable, newAct, &QAction::setEnabled);
    }
    if(option & noBar) return aMenu;
    aToolBar->addAction(newAct);
    return aMenu;
}

void MainWindow::close() {
    QWidget::close();
}

void MainWindow::cut() {
    textEdit->cut();
}

void MainWindow::copy() {
    textEdit->copy();
}

void MainWindow::paste() {
    textEdit->paste();
}

void MainWindow::viewText() {
    //TODO
}

void MainWindow::aboutQt() {
    qApp->aboutQt();
}

void MainWindow::createActions() {
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
            tr("Save the document under a new name"), noBar)->addSeparator();//no bar entry
    addMenu(nullptr, &MainWindow::close,
            "application-exit", tr("E&xit"), QKeySequence::Quit,
            tr("Exit the application"), noBar);//no bar entry
    menuBar()->addSeparator();

#ifndef QT_NO_CLIPBOARD
    addMenu(tr("&Edit"), &MainWindow::cut,
            "edit-cut",  tr("Cu&t"), QKeySequence::Cut,
            tr("Cut the current selection's contents to the clipboard"), canCopy);
    addMenu(nullptr, &MainWindow::copy,
            "edit-copy", tr("&Copy"), QKeySequence::Copy,
            tr("Copy the current selection's contents to the clipboard"), canCopy);
    addMenu(nullptr, &MainWindow::paste,
            "edit-paste", tr("&Paste"), QKeySequence::Paste,
            tr("Paste the clipboard's contents into the current selection"));
    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

    addMenu(tr("&View"), &MainWindow::viewText,
            "view-text", tr("&Text"), QKeySequence::AddTab,
            tr("Show editable text view"));
    menuBar()->addSeparator();

    addMenu(tr("&Sync"), &MainWindow::publish,
            "sync-publish", tr("&Publish"), QKeySequence::Print,
            tr("Publish with services"));
    addMenu(nullptr, &MainWindow::subscribe,
            "sync-read", tr("&Read"), QKeySequence(Qt::CTRL + Qt::Key_R),
            tr("Read from services"))->addSeparator();
    addMenu(nullptr, &MainWindow::setRepo,
            nullptr, tr("&Set Sync..."), QKeySequence(Qt::CTRL + Qt::Key_H),
            tr("Publish with services"));
    menuBar()->addSeparator();

    addMenu(tr("&Help"), &MainWindow::about,
            nullptr, tr("&About"), 0,
            tr("Show the application's About box"));
    addMenu(nullptr, &MainWindow::aboutQt,
            nullptr, tr("About &Qt"), 0,
            tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
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
    directory = settings.value("directory", "").toString();
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("directory", directory);
}

bool MainWindow::maybeSave() {
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
        return saved;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
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

bool MainWindow::saveFile(const QString &fileName) {
    QString name;
    if(QString::compare(QFileInfo(fileName).suffix(), "txt", Qt::CaseInsensitive) != 0) {
        name = QString(fileName + ".txt");
    } else {
        name = fileName;
    }
    QFile file(name);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("File Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(name),
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

    setCurrentFile(name);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";//default to text
    //TODO: set default as note taking
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager) {
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
