#include "MainWindow.h"
#include "GraphParser.h"
#include "ForceDirectedLayout.h"
#include "SvgRenderer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QResizeEvent>
#include <QPainter>
#include <QPaintEvent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , graphReady_(false)
{
    setupUi();
    applyStyles();
}

void MainWindow::setupUi() {
    setWindowTitle("Визуализатор графов");
    setMinimumSize(600, 500);
    resize(900, 700);

    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    auto* titleLabel = new QLabel("Визуализатор графов", this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    textEdit_ = new QTextEdit(this);
    textEdit_->setObjectName("graphInput");
    textEdit_->setPlaceholderText(
        "Введите описание графа здесь...\n\n"
        "Формат:\n"
        "• Первая строка — количество вершин\n"
        "• Вторая строка — метки вершин через пробел\n"
        "• Дальше — пары вершин (рёбра), по одной на строку\n\n"
        "Пример:\n"
        "5\n"
        "A B C D E\n"
        "A B\n"
        "B C\n"
        "C D\n"
        "D E\n"
        "E A");
    textEdit_->setMaximumHeight(160);
    textEdit_->setFont(QFont("Consolas", 11));
    mainLayout->addWidget(textEdit_);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);

    loadButton_ = new QPushButton("📂 Загрузить файл", this);
    loadButton_->setObjectName("loadButton");
    visualizeButton_ = new QPushButton("🎨 Визуализировать", this);
    visualizeButton_->setObjectName("visualizeButton");
    saveButton_ = new QPushButton("💾 Сохранить SVG", this);
    saveButton_->setObjectName("saveButton");

    buttonLayout->addWidget(loadButton_);
    buttonLayout->addWidget(visualizeButton_);
    buttonLayout->addWidget(saveButton_);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    canvas_ = new Canvas(this);
    canvas_->setObjectName("canvas");
    mainLayout->addWidget(canvas_, 1);

    statusLabel_ = new QLabel("Готов к работе", this);
    statusLabel_->setObjectName("statusLabel");
    mainLayout->addWidget(statusLabel_);

    connect(loadButton_, &QPushButton::clicked,
            this, &MainWindow::onLoadFile);
    connect(visualizeButton_, &QPushButton::clicked,
            this, &MainWindow::onVisualize);
    connect(saveButton_, &QPushButton::clicked,
            this, &MainWindow::onSaveSvg);
}

void MainWindow::applyStyles() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f2f5;
        }

        #titleLabel {
            font-size: 18px;
            font-weight: bold;
            color: #1a1a2e;
            padding: 4px;
        }

        #graphInput {
            background-color: #ffffff;
            border: 1px solid #d0d5dd;
            border-radius: 8px;
            padding: 8px;
            font-size: 13px;
            color: #333333;
        }

        #graphInput:focus {
            border: 1px solid #4a90d9;
        }

        QPushButton {
            background-color: #4a90d9;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #3a7bc8;
        }

        QPushButton:pressed {
            background-color: #2d6cb5;
        }

        #loadButton {
            background-color: #6c757d;
        }

        #loadButton:hover {
            background-color: #5a6268;
        }

        #saveButton {
            background-color: #28a745;
        }

        #saveButton:hover {
            background-color: #218838;
        }

        #statusLabel {
            color: #666666;
            font-size: 12px;
            padding: 2px 4px;
        }
    )");
}

void MainWindow::onLoadFile() {
    QString filename = QFileDialog::getOpenFileName(
        this, "Открыть файл с графом", QString(),
        "Текстовые файлы (*.txt);;Все файлы (*)");

    if (filename.isEmpty()) {
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка",
                             "Не удалось открыть файл: " + filename);
        statusLabel_->setText("Ошибка загрузки файла");
        return;
    }

    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();

    textEdit_->setPlainText(content);
    statusLabel_->setText("Файл загружен: " + filename);
}

void MainWindow::onVisualize() {
    QString text = textEdit_->toPlainText().trimmed();

    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Введите описание графа или загрузите файл.");
        return;
    }

    try {
        statusLabel_->setText("Парсинг графа...");
        graph_ = parser_.parse(text);

        statusLabel_->setText("Раскладка графа...");
        auto& layout = ForceDirectedLayout::getInstance();
        layout.run(graph_);

        graphReady_ = true;
        canvas_->setGraph(&graph_);
        statusLabel_->setText(
            "Готово. Вершин: " + QString::number(graph_.getVertexCount()) +
            ", рёбер: " + QString::number(graph_.getEdgeCount()));
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        statusLabel_->setText("Ошибка визуализации");
        graphReady_ = false;
    }
}

void MainWindow::onSaveSvg() {
    if (!graphReady_) {
        QMessageBox::warning(this, "Ошибка",
                             "Сначала визуализируйте граф.");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this, "Сохранить SVG", "graph.svg",
        "SVG файлы (*.svg);;Все файлы (*)");

    if (filename.isEmpty()) {
        return;
    }

    try {
        renderer_.render(graph_, filename);
        statusLabel_->setText("Сохранено: " + filename);
        QMessageBox::information(this, "Успех",
                                 "Граф сохранён в " + filename);
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
        statusLabel_->setText("Ошибка сохранения");
    }
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if (graphReady_) {
        canvas_->update();
    }
}
