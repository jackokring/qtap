#ifndef STATSVIEW_H
#define STATSVIEW_H

#include "aviewwidget.h"
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class StatsView;
}

class StatsView : public AViewWidget {
    Q_OBJECT

public:
    explicit StatsView(QWidget *parent);
    ~StatsView();
    QString getViewName() override;
    QString getIconName() override;
    QKeySequence getShortCut() override;
    void checkAvailable(bool saved) override;//is view available
    void clear() override;//clear on new document
    void create() override;//run on show
    bool needsSave() override;//needs disk save (modified?)
    bool canCache() override;//can load from disk
    QString getExtension() override;//what to extend as
    QString blockingSave() override;//save to disk this string
    void cacheLoad(QString input) override;//load from disk accept
    void setCommands() override;//install menu through addMenu()
    void readSettings(QSettings *settings) override;
    //void writeSettings(QSettings *settings) override;
    bool hasRegenerate() override;//can regenerate .txt
    QString regenerate() override;//regenerate .txt
    bool canCut() override;//can currently cut
    bool canCopy() override;//can currently copy
    bool canPaste() override;//can currently paste
    bool canUndo() override;//can currently undo
    bool canRedo() override;//can currently redo
    void cut() override;
    void copy() override;
    void paste() override;
    void undo() override;
    void redo() override;

private:
    Ui::StatsView *ui;
};

#endif // STATSVIEW_H
