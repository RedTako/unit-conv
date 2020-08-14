#include "MainWindow.hpp"
#include <iostream>
#include <unordered_set>
#include <algorithm>

void value_box_validator(QLineEdit& box, const QString& text)
{
    const static std::unordered_set<char> allowed_characters = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
        '+', '-', '.'
    };

    QString buffer = "";
    bool needs_replacing = false;
    for(auto& c : text)
    {
        if(allowed_characters.count(c.toLatin1()) > 0)
        {
            buffer.append(c);
        }
        else
        {
            needs_replacing = true;
        }
        
    }

    if(needs_replacing)
    {
        box.setText(buffer);
    }
}

void MainWindow::update_unit_ui()
{
    ui_updating = true;
    for(auto& entry : unit_categories)
    {
        auto category_name = entry.first;
        category_combo->addItem(category_name.c_str());
    }
    ui_updating = false;
}

void MainWindow::on_category_change(const QString& category, QComboBox& left, QComboBox& right)
{
    ui_updating = true;
    auto& unit_map = unit_categories[category.toStdString()];
    
    left.clear();
    right.clear();
    
    QStringList unit_list;

    for(auto& entry : unit_map)
    {
        unit_list.push_back(QString::fromStdString(entry.first));
    }
    unit_list.sort();
    left.addItems(unit_list);
    right.addItems(unit_list);
    ui_updating = false;
}

void MainWindow::unit_convert(const UnitPair& src, UnitPair& dst)
{
    auto& unit_map = unit_categories[category_combo->currentText().toStdString()];

    auto& src_unit = unit_map[src.unit.currentText().toStdString()];
    auto& dst_unit = unit_map[dst.unit.currentText().toStdString()];

    bool success;
    auto src_value = src.text.text().toDouble(&success);
    if(!success) return;
    auto result = src_unit->convert_to(*dst_unit, src_value);

    auto dst_text = std::to_string(result);
    dst_text.erase( dst_text.find_last_not_of('0') + 1);
    if(dst_text[dst_text.size() - 1] == '.')
        dst_text.erase(dst_text.size() - 1);
    
    dst.text.setText(QString::fromStdString(dst_text));
}

void MainWindow::convert_left()
{
    if(ui_updating) return;
    UnitPair src{*value_box_1, *unit_combo_1};
    UnitPair dst{*value_box_2, *unit_combo_2};
    unit_convert(src, dst); 
}

void MainWindow::convert_right()
{
    if(ui_updating) return;
    UnitPair dst{*value_box_1, *unit_combo_1};
    UnitPair src{*value_box_2, *unit_combo_2};
    unit_convert(src, dst); 
}

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
    setupUi(this);
    std::string unit_file_path = QApplication::applicationDirPath().toStdString() + "/units.txt";
    std::ifstream units_file(unit_file_path);
    unit_categories = read_units_file(units_file);
    update_unit_ui();
    on_category_change(category_combo->currentText(), *unit_combo_1, *unit_combo_2);

    connect(value_box_1, &QLineEdit::textEdited, [&](const QString& text)
    { 
        value_box_validator(*value_box_1, text); 
        convert_left();
    });
    connect(value_box_2, &QLineEdit::textEdited, [&](const QString& text)
    { 
        value_box_validator(*value_box_2, text); 
        convert_right();
    });
    connect(category_combo, &QComboBox::currentTextChanged, [&](const QString& text)
    {
        if(!ui_updating)
        { 
            on_category_change(text, *unit_combo_1, *unit_combo_2); 
            value_box_1->clear();
            value_box_2->clear();
        }
    });
    
    connect(unit_combo_1, &QComboBox::currentTextChanged, [&](const QString& text){ convert_right(); });
    connect(unit_combo_2, &QComboBox::currentTextChanged, [&](const QString& text){ convert_left(); });
}

MainWindow::~MainWindow()
{
}