#include "gui.hpp"
#include "config.hpp"

#include <QApplication>
#include <QWidget>
#include <QTabWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLayout>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

void gui(int argc, char* argv[]) { 
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("GNU/Linux CS2 Hack");
  window.setGeometry(100, 100, 300, 200);
  window.setFixedSize(300, 200);
  
  QTabWidget* tabs = new QTabWidget(&window);
  tabs->setGeometry(0, 0, 300, 200);
  
  QWidget* esp_tab = new QWidget;
  
  QVBoxLayout* esp_layout = new QVBoxLayout(esp_tab);

  QCheckBox* esp_master = new QCheckBox("Master", esp_tab);
  esp_master->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_master);
  QObject::connect(esp_master, &QPushButton::clicked, []() {
    config.esp.master = !config.esp.master;
  });
  
  QCheckBox* esp_box = new QCheckBox("Box", esp_tab);
  esp_box->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_box);
  QObject::connect(esp_box, &QPushButton::clicked, []() {
    config.esp.box = !config.esp.box;
  });

  QCheckBox* esp_health = new QCheckBox("Health", esp_tab);
  esp_health->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_health);
  QObject::connect(esp_health, &QPushButton::clicked, []() {
    config.esp.health = !config.esp.health;
  });

  QCheckBox* esp_skeleton = new QCheckBox("Skeleton", esp_tab);
  esp_skeleton->setCheckState(Qt::CheckState::Unchecked);
  esp_layout->addWidget(esp_skeleton);
  QObject::connect(esp_skeleton, &QPushButton::clicked, []() {
    config.esp.skeleton = !config.esp.skeleton;
  });
  
  QCheckBox* snap_lines = new QCheckBox("Snap lines", esp_tab);
  snap_lines->setCheckState(Qt::CheckState::Unchecked);
  esp_layout->addWidget(snap_lines);
  QObject::connect(snap_lines, &QPushButton::clicked, []() {
    config.esp.snap_lines = !config.esp.snap_lines;
  });

  tabs->addTab(esp_tab, "ESP");

  QWidget* aim_tab = new QWidget;
  
  QVBoxLayout* aim_layout = new QVBoxLayout(aim_tab);
    
  QCheckBox* aim_master = new QCheckBox("Master", esp_tab);
  aim_master->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_master);
  QObject::connect(aim_master, &QPushButton::clicked, []() {
    config.aim.master = !config.aim.master;
  });

  QCheckBox* aim_recoil = new QCheckBox("Recoil control", esp_tab);
  aim_recoil->setCheckState(Qt::CheckState::Checked);
  aim_layout->addWidget(aim_recoil);
  QObject::connect(aim_recoil, &QPushButton::clicked, []() {
    config.aim.recoil = !config.aim.recoil;
  });  
  
  tabs->addTab(aim_tab, "AIMBOT");
  
  window.show();

  app.exec();

  return;  
}
