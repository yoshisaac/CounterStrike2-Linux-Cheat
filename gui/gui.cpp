#include "gui.hpp"
#include "config.hpp"

#include <QApplication>
#include <QColorDialog>
#include <QKeySequence>
#include <QKeySequenceEdit>
#include <QWidget>
#include <QTabWidget>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLayout>
#include <QKeyEvent>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>

#define WIDTH 300
#define HEIGHT 250

void gui(int argc, char* argv[]) { 
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("GNU/Linux CS2 Hack");
  window.setGeometry(100, 100, WIDTH, HEIGHT);
  window.setFixedSize(WIDTH, HEIGHT);
  
  QTabWidget* tabs = new QTabWidget(&window);
  tabs->setGeometry(0, 0, WIDTH, HEIGHT);

  /* ESP begin */
  QWidget* esp_tab = new QWidget;
  
  QVBoxLayout* esp_layout = new QVBoxLayout(esp_tab);

  QCheckBox* esp_master = new QCheckBox("Master", esp_tab);
  esp_master->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_master);
  QObject::connect(esp_master, &QPushButton::clicked, []() {
    config.esp.master = !config.esp.master;
  });

  /* Box begin */
  QHBoxLayout* box_layout = new QHBoxLayout();
  QCheckBox* esp_box = new QCheckBox("Box", esp_tab);
  esp_box->setCheckState(Qt::CheckState::Checked);
  box_layout->addWidget(esp_box);
  QObject::connect(esp_box, &QPushButton::clicked, []() {
    config.esp.box = !config.esp.box;
  });

  QWidget box_color_window;
  box_color_window.setWindowTitle("Select Box Color");
  box_color_window.setGeometry(100, 100, 520, 450);
  box_color_window.setFixedSize(520, 450);

  QVBoxLayout* box_color_dialog_layout = new QVBoxLayout(&box_color_window);

  QColorDialog* box_color_dialog = new QColorDialog(QColor(255, 0, 50), esp_tab);
  QPushButton* box_color_button = new QPushButton();
  box_color_button->setAutoFillBackground(true);
  box_color_button->setPalette(QColor(255, 0, 50));
  box_color_button->update();
  box_color_dialog_layout->addWidget(box_color_dialog);
  box_color_dialog->setWindowFlags(Qt::Widget);
  box_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  box_layout->addWidget(box_color_button);
  QObject::connect(box_color_button, &QPushButton::clicked, [&]() {
    box_color_window.show();
  });

  QObject::connect(box_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.box_color[0], &config.esp.box_color[1], &config.esp.box_color[2]);
    box_color_button->setPalette(color);
    box_color_button->update();
  });
 
  esp_layout->addLayout(box_layout);
  /* Box end */

  QCheckBox* esp_health = new QCheckBox("Health", esp_tab);
  esp_health->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_health);
  QObject::connect(esp_health, &QPushButton::clicked, []() {
    config.esp.health = !config.esp.health;
  });

  QCheckBox* esp_name = new QCheckBox("Name", esp_tab);
  esp_name->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_name);
  QObject::connect(esp_name, &QPushButton::clicked, []() {
    config.esp.name = !config.esp.name;
  });  

  /* Skeleton begin */
  QHBoxLayout* skeleton_layout = new QHBoxLayout();
  QCheckBox* esp_skeleton = new QCheckBox("Skeleton", esp_tab);
  esp_skeleton->setCheckState(Qt::CheckState::Unchecked);
  skeleton_layout->addWidget(esp_skeleton);
  QObject::connect(esp_skeleton, &QPushButton::clicked, []() {
    config.esp.skeleton = !config.esp.skeleton;
  });

  QWidget skeleton_color_window;
  skeleton_color_window.setWindowTitle("Select Skeleton Color");
  skeleton_color_window.setGeometry(100, 100, 520, 450);
  skeleton_color_window.setFixedSize(520, 450);

  QVBoxLayout* skeleton_color_dialog_layout = new QVBoxLayout(&skeleton_color_window);

  QColorDialog* skeleton_color_dialog = new QColorDialog(QColor(255, 255, 255), esp_tab);
  QPushButton* skeleton_color_button = new QPushButton();
  skeleton_color_button->setAutoFillBackground(true);
  skeleton_color_button->setPalette(QColor(255, 255, 255));
  skeleton_color_button->update();
  skeleton_color_dialog_layout->addWidget(skeleton_color_dialog);
  skeleton_color_dialog->setWindowFlags(Qt::Widget);
  skeleton_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  skeleton_layout->addWidget(skeleton_color_button);
  QObject::connect(skeleton_color_button, &QPushButton::clicked, [&]() {
    skeleton_color_window.show();
  });

  QObject::connect(skeleton_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.skeleton_color[0], &config.esp.skeleton_color[1], &config.esp.skeleton_color[2]);
    skeleton_color_button->setPalette(color);
    skeleton_color_button->update();
  });
 
  esp_layout->addLayout(skeleton_layout);
  /* Skeleton end */
  
  QCheckBox* snap_lines = new QCheckBox("Snap lines", esp_tab);
  snap_lines->setCheckState(Qt::CheckState::Unchecked);
  esp_layout->addWidget(snap_lines);
  QObject::connect(snap_lines, &QPushButton::clicked, []() {
    config.esp.snap_lines = !config.esp.snap_lines;
  });

  tabs->addTab(esp_tab, "ESP");
  /* ESP end */
  
  QWidget* aim_tab = new QWidget;
  
  QVBoxLayout* aim_layout = new QVBoxLayout(aim_tab);
    
  QCheckBox* aim_master = new QCheckBox("Master", esp_tab);
  aim_master->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_master);
  QObject::connect(aim_master, &QPushButton::clicked, []() {
    config.aim.master = !config.aim.master;
  });

  QKeySequenceEdit* key = new QKeySequenceEdit;
  key->setKeySequence(QKeySequence("c"));
  aim_layout->addWidget(key);
  QObject::connect(key, &QKeySequenceEdit::keySequenceChanged, [](const QKeySequence &keySequence) {
    config.aim.key = keySequence.toString().toStdString()[0];
  });
  
  QCheckBox* aim_recoil = new QCheckBox("Recoil control", esp_tab);
  aim_recoil->setCheckState(Qt::CheckState::Checked);
  aim_layout->addWidget(aim_recoil);
  QObject::connect(aim_recoil, &QPushButton::clicked, []() {
    config.aim.recoil = !config.aim.recoil;
  });  
  
  tabs->addTab(aim_tab, "AIMBOT");
  
  QWidget* misc_tab = new QWidget;
  
  QVBoxLayout* misc_layout = new QVBoxLayout(misc_tab);
  
  QCheckBox* spin = new QCheckBox("Spin", misc_tab);
  spin->setCheckState(Qt::CheckState::Unchecked);
  misc_layout->addWidget(spin);
  QObject::connect(spin, &QPushButton::clicked, []() {
    config.misc.spin = !config.misc.spin;
  });
  
  tabs->addTab(misc_tab, "MISC");
  
  window.show();
  
  app.exec(); //halts here until window is destroyed

  pthread_kill(pthread_self(), SIGINT);
  
  return;  
}
