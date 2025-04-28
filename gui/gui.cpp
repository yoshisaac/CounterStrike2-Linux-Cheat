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
  
  QCheckBox* esp_ignore_team = new QCheckBox("Ignore team", esp_tab);
  esp_ignore_team->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_ignore_team);
  QObject::connect(esp_ignore_team, &QPushButton::clicked, []() {
    config.esp.ignore_team = !config.esp.ignore_team;
  });  
  
  QCheckBox* esp_spotted = new QCheckBox("Spotted", esp_tab);
  esp_spotted->setCheckState(Qt::CheckState::Unchecked);
  esp_layout->addWidget(esp_spotted);
  QObject::connect(esp_spotted, &QPushButton::clicked, []() {
    config.esp.spotted = !config.esp.spotted;
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

  /* Health begin */
  QHBoxLayout* health_layout = new QHBoxLayout();
  QCheckBox* esp_health_bar = new QCheckBox("Health bar", esp_tab);
  esp_health_bar->setCheckState(Qt::CheckState::Checked);
  health_layout->addWidget(esp_health_bar);
  QObject::connect(esp_health_bar, &QPushButton::clicked, []() {
    config.esp.health_bar = !config.esp.health_bar;
  });
  
  QCheckBox* esp_health_text = new QCheckBox("Health text", esp_tab);
  esp_health_text->setCheckState(Qt::CheckState::Unchecked);
  health_layout->addWidget(esp_health_text);
  QObject::connect(esp_health_text, &QPushButton::clicked, []() {
    config.esp.health_text = !config.esp.health_text;
  });

  esp_layout->addLayout(health_layout);
  /* Health end */
  
  QCheckBox* esp_name = new QCheckBox("Name", esp_tab);
  esp_name->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_name);
  QObject::connect(esp_name, &QPushButton::clicked, []() {
    config.esp.name = !config.esp.name;
  });  

  QCheckBox* esp_weapon_name = new QCheckBox("Weapon name", esp_tab);
  esp_weapon_name->setCheckState(Qt::CheckState::Unchecked);
  esp_layout->addWidget(esp_weapon_name);
  QObject::connect(esp_weapon_name, &QPushButton::clicked, []() {
    config.esp.weapon_name = !config.esp.weapon_name;
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

  /* Head dot begin */
  QHBoxLayout* head_dot_layout = new QHBoxLayout();
  QCheckBox* esp_head_dot = new QCheckBox("Head dot", esp_tab);
  esp_head_dot->setCheckState(Qt::CheckState::Unchecked);
  head_dot_layout->addWidget(esp_head_dot);
  QObject::connect(esp_head_dot, &QPushButton::clicked, []() {
    config.esp.head_dot = !config.esp.head_dot;
  });

  QCheckBox* esp_head_dot_fill = new QCheckBox("Filled", esp_tab);
  esp_head_dot_fill->setCheckState(Qt::CheckState::Checked);
  head_dot_layout->addWidget(esp_head_dot_fill);
  QObject::connect(esp_head_dot_fill, &QPushButton::clicked, []() {
    config.esp.head_dot_fill = !config.esp.head_dot_fill;
  });

  QCheckBox* esp_head_cat_ears = new QCheckBox("Ears", esp_tab);
  esp_head_cat_ears->setCheckState(Qt::CheckState::Unchecked);
  head_dot_layout->addWidget(esp_head_cat_ears);
  QObject::connect(esp_head_cat_ears, &QPushButton::clicked, []() {
    config.esp.cat_ears = !config.esp.cat_ears;
  });  
  
  QWidget head_dot_color_window;
  head_dot_color_window.setWindowTitle("Select Head_Dot Color");
  head_dot_color_window.setGeometry(100, 100, 520, 450);
  head_dot_color_window.setFixedSize(520, 450);

  QVBoxLayout* head_dot_color_dialog_layout = new QVBoxLayout(&head_dot_color_window);

  QColorDialog* head_dot_color_dialog = new QColorDialog(QColor(255, 255, 255), esp_tab);
  QPushButton* head_dot_color_button = new QPushButton();
  head_dot_color_button->setAutoFillBackground(true);
  head_dot_color_button->setPalette(QColor(255, 255, 255));
  head_dot_color_button->update();
  head_dot_color_dialog_layout->addWidget(head_dot_color_dialog);
  head_dot_color_dialog->setWindowFlags(Qt::Widget);
  head_dot_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  head_dot_layout->addWidget(head_dot_color_button);
  QObject::connect(head_dot_color_button, &QPushButton::clicked, [&]() {
    head_dot_color_window.show();
  });

  QObject::connect(head_dot_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.head_dot_color[0], &config.esp.head_dot_color[1], &config.esp.head_dot_color[2]);
    head_dot_color_button->setPalette(color);
    head_dot_color_button->update();
  });
 
  esp_layout->addLayout(head_dot_layout);  
  /* Head dot end */
  
  /* Snap lines begin */
  QHBoxLayout* snap_lines_layout = new QHBoxLayout();
  QCheckBox* snap_lines = new QCheckBox("Snap lines", esp_tab);
  snap_lines->setCheckState(Qt::CheckState::Unchecked);
  snap_lines_layout->addWidget(snap_lines);
  QObject::connect(snap_lines, &QPushButton::clicked, []() {
    config.esp.snap_lines = !config.esp.snap_lines;
  });

  QWidget snap_lines_color_window;
  snap_lines_color_window.setWindowTitle("Select Snap Lines Color");
  snap_lines_color_window.setGeometry(100, 100, 520, 450);
  snap_lines_color_window.setFixedSize(520, 450);
  
  QVBoxLayout* snap_lines_color_dialog_layout = new QVBoxLayout(&snap_lines_color_window);

  QColorDialog* snap_lines_color_dialog = new QColorDialog(QColor(0, 255, 0), esp_tab);
  QPushButton* snap_lines_color_button = new QPushButton();
  snap_lines_color_button->setAutoFillBackground(true);
  snap_lines_color_button->setPalette(QColor(0, 255, 0));
  snap_lines_color_button->update();
  snap_lines_color_dialog_layout->addWidget(snap_lines_color_dialog);
  snap_lines_color_dialog->setWindowFlags(Qt::Widget);
  snap_lines_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  snap_lines_layout->addWidget(snap_lines_color_button);
  QObject::connect(snap_lines_color_button, &QPushButton::clicked, [&]() {
    snap_lines_color_window.show();
  });

  QObject::connect(snap_lines_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.snap_lines_color[0], &config.esp.snap_lines_color[1], &config.esp.snap_lines_color[2]);
    snap_lines_color_button->setPalette(color);
    snap_lines_color_button->update();
  });
 
  esp_layout->addLayout(snap_lines_layout);
  /* Snap lines end */

  tabs->addTab(esp_tab, "ESP");
  /* ESP end */

  /* Visuals begin */
  QWidget* visuals_tab = new QWidget;

  QVBoxLayout* visuals_layout = new QVBoxLayout(visuals_tab);

  /* Crosshair begin */
  QHBoxLayout* crosshair_layout = new QHBoxLayout();
  QCheckBox* crosshair = new QCheckBox("Crosshair", visuals_tab);
  crosshair->setCheckState(Qt::CheckState::Unchecked);
  crosshair_layout->addWidget(crosshair);
  QObject::connect(crosshair, &QPushButton::clicked, []() {
    config.visuals.crosshair = !config.visuals.crosshair;
  });
  
  QWidget crosshair_color_window;
  crosshair_color_window.setWindowTitle("Select Snap Lines Color");
  crosshair_color_window.setGeometry(100, 100, 520, 450);
  crosshair_color_window.setFixedSize(520, 450);
  
  QVBoxLayout* crosshair_color_dialog_layout = new QVBoxLayout(&crosshair_color_window);

  QColorDialog* crosshair_color_dialog = new QColorDialog(QColor(0, 255, 0), esp_tab);
  QPushButton* crosshair_color_button = new QPushButton();
  crosshair_color_button->setAutoFillBackground(true);
  crosshair_color_button->setPalette(QColor(0, 255, 0));
  crosshair_color_button->update();
  crosshair_color_dialog_layout->addWidget(crosshair_color_dialog);
  crosshair_color_dialog->setWindowFlags(Qt::Widget);
  crosshair_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  crosshair_layout->addWidget(crosshair_color_button);
  QObject::connect(crosshair_color_button, &QPushButton::clicked, [&]() {
    crosshair_color_window.show();
  });

  QObject::connect(crosshair_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.visuals.crosshair_color[0], &config.visuals.crosshair_color[1], &config.visuals.crosshair_color[2]);
    crosshair_color_button->setPalette(color);
    crosshair_color_button->update();
  });
 
  visuals_layout->addLayout(crosshair_layout);
  /* Crosshair end */

  QCheckBox* visualize_recoil = new QCheckBox("Visualize recoil", visuals_tab);
  visualize_recoil->setCheckState(Qt::CheckState::Checked);
  visuals_layout->addWidget(visualize_recoil);
  QObject::connect(visualize_recoil, &QPushButton::clicked, []() {
    config.visuals.visualize_recoil = !config.visuals.visualize_recoil;
  });

  
  QCheckBox* sniper_only = new QCheckBox("Sniper only", visuals_tab);
  sniper_only->setCheckState(Qt::CheckState::Checked);
  visuals_layout->addWidget(sniper_only);
  QObject::connect(sniper_only, &QPushButton::clicked, []() {
    config.visuals.sniper_only = !config.visuals.sniper_only;
  });

  QCheckBox* fps = new QCheckBox("Fps", visuals_tab);
  fps->setCheckState(Qt::CheckState::Unchecked);
  visuals_layout->addWidget(fps);
  QObject::connect(fps, &QPushButton::clicked, []() {
    config.visuals.fps = !config.visuals.fps;
  });
  
  tabs->addTab(visuals_tab, "VISUALS");
  /* Visuals end */
  
  /*Aimbot begin */
  QWidget* aim_tab = new QWidget;
  
  QVBoxLayout* aim_layout = new QVBoxLayout(aim_tab);
    
  QCheckBox* aim_master = new QCheckBox("Master", aim_tab);
  aim_master->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_master);
  QObject::connect(aim_master, &QPushButton::clicked, []() {
    config.aim.master = !config.aim.master;
  });

 QCheckBox* aim_ignore_team = new QCheckBox("Ignore team", aim_tab);
  aim_ignore_team->setCheckState(Qt::CheckState::Checked);
  aim_layout->addWidget(aim_ignore_team);
  QObject::connect(aim_ignore_team, &QPushButton::clicked, []() {
    config.aim.ignore_team = !config.aim.ignore_team;
  });
  
  QCheckBox* aim_spotted = new QCheckBox("Spotted", aim_tab);
  aim_spotted->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_spotted);
  QObject::connect(aim_spotted, &QPushButton::clicked, []() {
    config.aim.spotted = !config.aim.spotted;
  });
  
  /* Aimbot fov begin */
  QHBoxLayout* aim_fov_layout = new QHBoxLayout();
  QSlider* aim_fov_slider = new QSlider(Qt::Horizontal, aim_tab);
  aim_fov_slider->setRange(1, 180);
  aim_fov_slider->setValue(90);
  aim_fov_slider->setTickPosition(QSlider::NoTicks);
  aim_fov_layout->addWidget(aim_fov_slider); 

  QLabel* aim_fov_label = new QLabel(aim_tab);
  aim_fov_label->setText("FOV " + QString::number(aim_fov_slider->value()));
  aim_fov_layout->addWidget(aim_fov_label);

  QObject::connect(aim_fov_slider, &QSlider::valueChanged, [&](int value) {
    aim_fov_label->setText("FOV " + QString::number(aim_fov_slider->value()));
    config.aim.fov = value;
  });

  aim_layout->addLayout(aim_fov_layout);
  /* Aimbot fov end */
    
  QCheckBox* aim_show_fov = new QCheckBox("Show FOV", aim_tab);
  aim_show_fov->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_show_fov);
  QObject::connect(aim_show_fov, &QPushButton::clicked, []() {
    config.aim.show_fov = !config.aim.show_fov;
  });

  /* Aimbot snap lines begin */
  QHBoxLayout* aim_snap_lines_layout = new QHBoxLayout();
  QCheckBox* aim_snap_lines = new QCheckBox("Aimbot snap lines", aim_tab);
  aim_snap_lines->setCheckState(Qt::CheckState::Checked);
  aim_snap_lines_layout->addWidget(aim_snap_lines);
  QObject::connect(aim_snap_lines, &QPushButton::clicked, []() {
    config.aim.snap_lines = !config.aim.snap_lines;
  });  

  QWidget aim_snap_lines_color_window;
  aim_snap_lines_color_window.setWindowTitle("Select Snap Lines Color");
  aim_snap_lines_color_window.setGeometry(100, 100, 520, 450);
  aim_snap_lines_color_window.setFixedSize(520, 450);
  
  QVBoxLayout* aim_snap_lines_color_dialog_layout = new QVBoxLayout(&aim_snap_lines_color_window);

  QColorDialog* aim_snap_lines_color_dialog = new QColorDialog(QColor(255, 255, 0), esp_tab);
  QPushButton* aim_snap_lines_color_button = new QPushButton();
  aim_snap_lines_color_button->setAutoFillBackground(true);
  aim_snap_lines_color_button->setPalette(QColor(255, 255, 0));
  aim_snap_lines_color_button->update();
  aim_snap_lines_color_dialog_layout->addWidget(aim_snap_lines_color_dialog);
  aim_snap_lines_color_dialog->setWindowFlags(Qt::Widget);
  aim_snap_lines_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  aim_snap_lines_layout->addWidget(aim_snap_lines_color_button);
  QObject::connect(aim_snap_lines_color_button, &QPushButton::clicked, [&]() {
    aim_snap_lines_color_window.show();
  });

  QObject::connect(aim_snap_lines_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.aim.snap_lines_color[0], &config.aim.snap_lines_color[1], &config.aim.snap_lines_color[2]);
    aim_snap_lines_color_button->setPalette(color);
    aim_snap_lines_color_button->update();
  });
 
  aim_layout->addLayout(aim_snap_lines_layout);  
  /* Aimbot snap lines end */
  
  QKeySequenceEdit* key = new QKeySequenceEdit;
  key->setKeySequence(QKeySequence("c"));
  aim_layout->addWidget(key);
  QObject::connect(key, &QKeySequenceEdit::keySequenceChanged, [](const QKeySequence &keySequence) {
    config.aim.key = keySequence.toString().toStdString()[0];
  });
  
  QCheckBox* aim_recoil = new QCheckBox("Recoil control", aim_tab);
  aim_recoil->setCheckState(Qt::CheckState::Checked);
  aim_layout->addWidget(aim_recoil);
  QObject::connect(aim_recoil, &QPushButton::clicked, []() {
    config.aim.recoil = !config.aim.recoil;
  });  

  QCheckBox* aim_auto_shoot = new QCheckBox("Automatically shoot", aim_tab);
  aim_auto_shoot->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_auto_shoot);
  QObject::connect(aim_auto_shoot, &QPushButton::clicked, []() {
    config.aim.auto_shoot = !config.aim.auto_shoot;
  }); 
  
  tabs->addTab(aim_tab, "AIMBOT");
  /* Aimbot end */
  
  window.show();
  
  app.exec(); //halts here until window is destroyed

  pthread_kill(pthread_self(), SIGINT);
  
  return;  
}
