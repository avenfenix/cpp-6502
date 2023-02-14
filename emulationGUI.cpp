#include "emulationGUI.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <inttypes.h>
#include <bitset>
#include <QFileDialog>

#define HEX1_X 0
#define HEX2_X 1
#define HEX1_DOLAR 2
#define HEX2_DOLAR 3
#define HEX1 4
#define HEX2 5

std::string hex1byte0x(int x) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 4));
	return A_t;
}

std::string hex2byte0x(int x) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 6));
	return A_t;
}

std::string hex1bytedolar(int x) {
	std::stringstream stream;
	stream << "$" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 3));
	return A_t;
}

std::string hex2bytedolar(int x) {
	std::stringstream stream;
	stream << "$" << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 5));
	return A_t;
}

std::string hex1byte(int x) {
	std::stringstream stream;
	stream << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 2));
	return A_t;
}

std::string hex2byte(int x) {
	std::stringstream stream;
	stream << std::uppercase << std::hex << x << std::endl;
	std::string A_t(stream.str().substr(0, 4));
	return A_t;
}

std::string number2hex(int x, int out)
{
	if (out == 0)
	{
		return hex1byte0x(x);
	}
	else if (out == 1)
	{
		return hex2byte0x(x);
	}
	else if (out == 2)
	{
		return hex1bytedolar(x);
	}
	else if (out == 3)
	{
		return hex2bytedolar(x);
	}
	else if (out == 4)
	{
		return hex1byte(x);
	}
	else if (out == 5)
	{
		return hex2byte(x);
	}
	return "";
}


emulationGUI::emulationGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->userCreate();
    this->userUpdate();
}

emulationGUI::~emulationGUI()
{}


void emulationGUI::userCreate() {
    
	

	QObject::connect(ui.stepbystepButton, &QPushButton::clicked, [=]()
		{
			while (!DataBus.CPU.complete) DataBus.clock();
			DataBus.CPU.complete = false;
            userUpdate();

        });
	QObject::connect(ui.nextcpucycle, &QPushButton::clicked, [=]() {
			DataBus.clock();
			userUpdate();
			
		});
    QObject::connect(ui.resetButton, &QPushButton::clicked, [=]()
        {
            DataBus.reset();
            userUpdate();
        });
    QObject::connect(ui.controlButton, &QPushButton::clicked, [=]()
        {
			if (running) {
				ui.controlButton->setText("Run");

				ui.resetButton->setEnabled(true);
				ui.nextcpucycle->setEnabled(true);
				ui.stepbystepButton->setEnabled(true);

				running = false;
			}
			else {
				ui.controlButton->setText("Pause");
				ui.resetButton->setEnabled(false);
				ui.nextcpucycle->setEnabled(false);
				ui.stepbystepButton->setEnabled(false);
				running = true;
			}
        });


	QObject::connect(ui.actionAbrir, &QAction::triggered, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jana");
		qDebug() << fileName;
		DataBus.loadRom(fileName.toStdString());
		resetDebug();
		resetStackView();
		DataBus.reset();
		userUpdate();
	});

	
	ui.tableWidget->insertColumn(0);
	ui.tableWidget->insertColumn(1);
	ui.tableWidget->insertColumn(2);
	ui.tableWidget->setHorizontalHeaderLabels({"Address", "Byte", "Detail"});
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget->setColumnWidth(0, 75);
	ui.tableWidget->setColumnWidth(1, 40);
	ui.tableWidget->setColumnWidth(2, 144);

	ui.stackTable->insertColumn(0);
	ui.stackTable->insertColumn(1);
	ui.stackTable->setHorizontalHeaderLabels({"Address","Byte"});
	ui.stackTable->setColumnWidth(0, 60);
	ui.stackTable->setColumnWidth(1, 60);
	ui.stackTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	

	
	
	// Falta convertir string a uint8_t y uint16_t 

	/*QObject::connect(ui.aValue, &QLineEdit::textChanged, [=](const QString& texto)
		{
			DataBus.CPU.A =  (uint8_t) (std::stoi(texto.toStdString()));
		});
	QObject::connect(ui.xValue, &QLineEdit::textChanged, [=](const QString& texto)
		{
			DataBus.CPU.X = (uint8_t)(std::stoi(texto.toStdString()));
		});
	QObject::connect(ui.yValue, &QLineEdit::textChanged, [=](const QString& texto)
		{
			DataBus.CPU.Y = (uint8_t)(std::stoi(texto.toStdString()));
		});
	QObject::connect(ui.spValue, &QLineEdit::textChanged, [=](const QString& texto)
		{
			DataBus.CPU.SP = (uint8_t)(std::stoi(texto.toStdString()));
		});
	QObject::connect(ui.pcValue, &QLineEdit::textChanged, [=](const QString& texto)
		{
			DataBus.CPU.PC = (uint16_t)(std::stoi(texto.toStdString()));
		});*/


    for (int i = 0; i < ramdata.size(); i++)
    {
        DataBus.write(ramdata[i].address, ramdata[i].byte);
    }

}

void emulationGUI::userUpdate() {
    ui.xValue->setText(number2hex(DataBus.CPU.X, HEX1).c_str());
	ui.yValue->setText(number2hex(DataBus.CPU.Y, HEX1).c_str());
	ui.aValue->setText(number2hex(DataBus.CPU.A, HEX1).c_str());
	ui.pcValue->setText(number2hex(DataBus.CPU.PC, HEX2).c_str());
	ui.spValue->setText(number2hex(DataBus.CPU.SP, HEX1).c_str());

	ui.bbit->setChecked((DataBus.CPU.GetFlag(aven6502::B) == 0x01)?true:false);
	ui.cbit->setChecked((DataBus.CPU.GetFlag(aven6502::C) == 0x01) ? true : false);
	ui.dbit->setChecked((DataBus.CPU.GetFlag(aven6502::D) == 0x01) ? true : false);
	ui.ibit->setChecked((DataBus.CPU.GetFlag(aven6502::I) == 0x01) ? true : false);
	ui.nbit->setChecked((DataBus.CPU.GetFlag(aven6502::N) == 0x01) ? true : false);
	ui.vbit->setChecked((DataBus.CPU.GetFlag(aven6502::V) == 0x01) ? true : false);
	ui.ubit->setChecked((DataBus.CPU.GetFlag(aven6502::U) == 0x01) ? true : false);
	ui.zbit->setChecked((DataBus.CPU.GetFlag(aven6502::Z) == 0x01) ? true : false);

	ui.cpucycleValue->setText(std::to_string((int)DataBus.clock_counter).c_str());

	DrawRAM();


}



void emulationGUI::resetDebug() {
	for (int i = 0; i < 32768; i++)
	{
		ui.tableWidget->insertRow(i);
	}

	

	for (int i = 0; i < 32768; i++)
	{
		std::stringstream b;
		std::string addr; int ind1 = 0;
		if (PROGRAM_ADDRESS + i < 0xFFF4) {
			aven6502::instruction ins = DataBus.CPU.lista[DataBus.read(0x8000 + i)];
			ind1 = resolveRAM({ ins, (int16_t)(PROGRAM_ADDRESS + i)}, addr);
			b << ins.nombre << "  " << addr; ram = b.str(); b.str(std::string());

			QTableWidgetItem* item1 = new QTableWidgetItem;
			item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
			QTableWidgetItem* item2 = new QTableWidgetItem;
			item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
			QTableWidgetItem* item3 = new QTableWidgetItem;
			item3->setText(ram.c_str());
			ui.tableWidget->setItem(i, 0, item1);
			ui.tableWidget->setItem(i, 1, item2);
			ui.tableWidget->setItem(i, 2, item3);

			for (int h = 0; h < ind1; h++) {
				ui.tableWidget->hideRow(i + 1 + h);
			}

			i += ind1;
		}
		else {
			if (0x8000 + i == 0xFFF4) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt COP LSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			} else if (0x8000 + i == 0xFFF5) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt COP MSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFF8) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt ABORT LSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFF9) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt ABORT MSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFA) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt NMI LSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFB) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt NMI MSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFC) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt RESET LSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFD) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt RESET MSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFE) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt IRQ/BRK LSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
			else if (0x8000 + i == 0xFFFF) {
				QTableWidgetItem* item1 = new QTableWidgetItem;
				item1->setText(number2hex(0x8000 + i, HEX2_X).c_str());
				QTableWidgetItem* item2 = new QTableWidgetItem;
				item2->setText(number2hex(DataBus.read(0x8000 + i), HEX1_X).c_str());
				QTableWidgetItem* item3 = new QTableWidgetItem;
				item3->setText("Interrupt IRQ/BRK MSB");
				ui.tableWidget->setItem(i, 0, item1);
				ui.tableWidget->setItem(i, 1, item2);
				ui.tableWidget->setItem(i, 2, item3);
			}
		}
		
	}
}

void emulationGUI::resetStackView()
{
	for (int i = 0; i < 256; i++)
	{
		ui.stackTable->insertRow(i);
	}

	for (int i = 0; i < 256; i++)
	{
		QTableWidgetItem* item1 = new QTableWidgetItem;
		item1->setText(number2hex(STACK_BASE - i, HEX2_X).c_str());
		ui.stackTable->setItem(i, 0, item1);

		QTableWidgetItem* item2 = new QTableWidgetItem;
		item2->setText(number2hex(DataBus.read(STACK_BASE - i), HEX1_X).c_str());
		ui.stackTable->setItem(i, 1, item2);
		

		
	}
}



int emulationGUI::resolveRAM(resolveStruct args, std::string& addr) {
	int ind = 0;

	std::string xd = "";

	if (args.ins.address == &aven6502::IMM) {

		addr = "#";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::REL) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::IMP) {
		addr = " ( IMP )";
		ind = 0;
	}
	if (args.ins.address == &aven6502::ABS) {

		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		ind = 2;
	}
	if (args.ins.address == &aven6502::ABX) {
		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ", X ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::ABY) {
		addr = "";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ", Y ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::ZP0) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		ind = 1;
	}
	if (args.ins.address == &aven6502::ZPX) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", X";
		ind = 1;
	}
	if (args.ins.address == &aven6502::ZPY) {
		addr = "";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", Y";
		ind = 1;
	}
	if (args.ins.address == &aven6502::IND) {
		addr = "(";
		uint16_t lsb = DataBus.read(args.pc + 1);
		uint16_t msb = DataBus.read(args.pc + 2);
		xd = number2hex(lsb + (msb << 8), 3);
		addr += xd;
		addr += ") ";
		ind = 2;
	}
	if (args.ins.address == &aven6502::IDX) {
		addr = "(";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += ", X)";
		ind = 1;
	}
	if (args.ins.address == &aven6502::IDY) {
		addr = "(";
		xd = number2hex(DataBus.read(args.pc + 1), 2);
		addr += xd;
		addr += "), Y";
		ind = 1;
	}

	return ind;
}

void emulationGUI::DrawRAM() {
	
	ui.tableWidget->selectRow((int)(DataBus.CPU.PC - 0x8000));
	/*QTableWidgetItem* item1 = ui.tableWidget->takeItem((int)(DataBus.CPU.PC - 0x8000), 0);
	QTableWidgetItem* item2 = ui.tableWidget->takeItem((int)(DataBus.CPU.PC - 0x8000), 1);
	QTableWidgetItem* item3 = ui.tableWidget->takeItem((int)(DataBus.CPU.PC - 0x8000), 2);

	item1->setForeground(Qt::red);
	ui.tableWidget->setItem((int)(DataBus.CPU.PC - 0x8000), 0, item1);*/
}