#pragma once
#include <QMainWindow>
#include "ui_unit_conv.h"
#include "Units.hpp"

struct UnitPair
{
    QLineEdit& text;
    QComboBox& unit;
};

class MainWindow: public QMainWindow, protected Ui::MainWindow
{
private:
    CategoryMap unit_categories;
    void update_unit_ui();
    void on_category_change(const QString& category, QComboBox& left, QComboBox& right);
    void unit_convert(const UnitPair& src, UnitPair& dst);

    void convert_left();
    void convert_right();
    bool ui_updating = false;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};