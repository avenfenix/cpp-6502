#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_emulationGUI.h"
#include <string>
#include "Bus.h"

class emulationGUI : public QMainWindow
{
    Q_OBJECT

public:
    emulationGUI(QWidget *parent = nullptr);
    ~emulationGUI();

public:
   void userCreate();
   void userUpdate();
   Bus DataBus;
   struct memformat {
       uint16_t address;
       uint8_t byte;
   };
   std::vector<memformat> ramdata =
   {
       {0x1FFE, 0x00}, {0x1FFF, 0x80},
       {0x2001, 0x23},
       {0x2002, 0x24},
       {0x2003, 0x25},
       {0x0080, 0x26},
       {0x0081, 0x27},
       {0x0082, 0x00}, {0x0083, 0x30},
       {0x0084, 0x00}, {0x0085, 0x30},
       {0x3000, 0x28}, {0x3001, 0x20},
       {0x3002, 0x29},

   };

   bool running = false;

public:
    struct resolveStruct {
        aven6502::instruction ins;
        int16_t pc;
    };
    std::string ram;
    void DrawRAM();
    void resetDebug();
    void resetStackView();
    int resolveRAM(resolveStruct args, std::string& addr);


public:
    Ui::emulationGUIClass ui;
};
