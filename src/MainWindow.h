#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Graph.h"
#include "GraphParser.h"
#include "SvgRenderer.h"
#include "Canvas.h"

class QTextEdit;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onLoadFile();
    void onVisualize();
    void onSaveSvg();

private:
    void setupUi();
    void applyStyles();

    QTextEdit* textEdit_;
    QPushButton* loadButton_;
    QPushButton* visualizeButton_;
    QPushButton* saveButton_;
    QLabel* statusLabel_;
    Canvas* canvas_;

    Graph graph_;
    GraphParser parser_;
    SvgRenderer renderer_;
    bool graphReady_;
};

#endif // MAINWINDOW_H
