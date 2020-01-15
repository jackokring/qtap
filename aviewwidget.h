#ifndef AVIEWWIDGET_H
#define AVIEWWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QSettings>
#include <QProgressDialog>

class AViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AViewWidget(QWidget *parent = nullptr);

    //===================================================
    // GUI
    //===================================================
    virtual QString getViewName();
    virtual QString getIconName();
    virtual QKeySequence getShortCut();
    QString getToolTipHelp();
    void setMainWindow(QMainWindow *mw);

    //===================================================
    // STATE
    //===================================================
    virtual void checkAvailable(bool saved = false);//is view available
    virtual void recycle();//recycle before new document
    virtual void clear();//clear on new document
    virtual void create();//run on show
    virtual bool needsSave();//needs disk save (modified?)
    virtual bool canCache();//can load from disk
    virtual QString getExtension();//what to extend as
    virtual QString blockingSave();//save to disk this string
    virtual void cacheLoad(QString input);//load from disk accept
    virtual void setCommands();//install menu through addMenu()
    virtual void readSettings(QSettings *settings);
    //virtual void writeSettings(QSettings *settings);
    virtual bool hasRegenerate();//can regenerate .txt base
    virtual QString regenerate();//regenerate .txt base

    //===================================================
    // ACTION
    //===================================================
    virtual bool canCut();//can currently cut
    virtual bool canCopy();//can currently copy
    virtual bool canPaste();//can currently paste
    virtual bool canUndo();//can currently undo
    virtual bool canRedo();//can currently redo
    virtual void cut();
    virtual void copy();
    virtual void paste();
    virtual void undo();
    virtual void redo();

    //===================================================
    // USEFUL
    //===================================================
    QWidget *focused();
    void progress100(QString message, QString cancel = tr("Cancel"));
    bool setProgress(int percent);//bool is cancelled
    QMainWindow *getMain();

public slots:
    void selectView();
    QString getTextFromMain();
private:
    template<class slotz>
    void addMenu(void(slotz::*fp)() = nullptr,
                 QString named = nullptr,
                 QString entry = nullptr,
                 QKeySequence shorty = 0,
                 QString help = nullptr);
    QMainWindow *main;
    QProgressDialog *progress = nullptr;

signals:
    void setAvailable(bool isAvail);

};

#endif // AVIEWWIDGET_H
