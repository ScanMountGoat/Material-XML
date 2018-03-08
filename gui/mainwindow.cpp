#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "src/materialxml.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Enable/disable the different search options.
	// Containers (QWidget) are used to hide/unhide all child widgets.
	ui->srcContainer->setEnabled(ui->srcCheckBox->isChecked());
	ui->dstContainer->setEnabled(ui->dstCheckBox->isChecked());
	ui->flagsContainer->setEnabled(ui->flagsCheckBox->isChecked());
	ui->matPropContainer->setEnabled(ui->matPropCheckBox->isChecked());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Xml", ".", "Xml files (*.xml)");
	MaterialXml::addMaterialsFromXML(fileName, searchSettings);

	displayMaterials();
}

void MainWindow::on_actionOpen_Folder_triggered()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		if (it.next().endsWith(".xml")) {
			QString fileName = it.next();
			MaterialXml::addMaterialsFromXML(fileName, searchSettings);
		}
	}

	displayMaterials();
}

void MainWindow::on_flagsCheckBox_clicked()
{
	bool isChecked = ui->flagsCheckBox->isChecked();
	ui->flagsContainer->setEnabled(isChecked);
	searchSettings.filterFlags = isChecked;
}

void MainWindow::on_dstCheckBox_clicked()
{
	bool isChecked = ui->dstCheckBox->isChecked();
	ui->dstContainer->setEnabled(isChecked);
	searchSettings.filterDst = isChecked;
}

void MainWindow::on_srcCheckBox_clicked()
{
	bool isChecked = ui->srcCheckBox->isChecked();
	ui->srcContainer->setEnabled(isChecked);
	searchSettings.filterSrc = isChecked;
}

bool MainWindow::hasValidSrc(Material material)
{
	int index = ui->srcOpComboBox->currentIndex();
	SearchSettings::ComparisonOperation comparison = (SearchSettings::ComparisonOperation) index;
	return SearchSettings::matchesSearch(comparison, material.srcFactor, searchSettings.srcFactor);
}

void MainWindow::displayMaterials()
{
	// clear the text display for each new search
	ui->plainTextEdit->clear();

	for (int i = 0; i < searchSettings.materialList.length(); i++) {
		Material material = searchSettings.materialList.at(i);

		bool validFlags = true;
		bool validSrc = true;
		bool validDst = true;
		bool validMatProp = true;

		// Check flags using the selected flags values and comparison operator.
		if (searchSettings.filterFlags) {
			int index = ui->flagsOpComboBox->currentIndex();
			SearchSettings::ComparisonOperation comparison = (SearchSettings::ComparisonOperation) index;

			switch (comparison) {
			case SearchSettings::ComparisonOperation::equals:
				validFlags = (material.flags & searchSettings.flags1) == searchSettings.flags2;
				break;
			case SearchSettings::ComparisonOperation::greater:
				validFlags = (material.flags & searchSettings.flags1) > searchSettings.flags2;
				break;
			case SearchSettings::ComparisonOperation::gEqual:
				validFlags = (material.flags & searchSettings.flags1) >= searchSettings.flags2;
				break;
			case SearchSettings::ComparisonOperation::less:
				validFlags = (material.flags & searchSettings.flags1) < searchSettings.flags2;
				break;
			case SearchSettings::ComparisonOperation::lEqual:
				validFlags = (material.flags & searchSettings.flags1) <= searchSettings.flags2;
				break;
			}
		}

		if (searchSettings.filterSrc) {
			validSrc = hasValidSrc(material);
		}

		if (searchSettings.filterDst) {
			int index = ui->dstOpComboBox->currentIndex();
			SearchSettings::ComparisonOperation comparison = (SearchSettings::ComparisonOperation) index;
			validDst = SearchSettings::matchesSearch(comparison, material.dstFactor, searchSettings.dstFactor);
		}

		if (searchSettings.filterPropertyName) {
			validMatProp = material.properties.contains("NU_" + searchSettings.materialProperty);
		}

		if (validFlags && validSrc && validDst && validMatProp) {
			ui->plainTextEdit->appendPlainText(material.fileName);

			if (searchSettings.filterFlags) {
				QString flags;
				ui->plainTextEdit->appendPlainText(flags.setNum(material.flags, 16));
			}

			if (searchSettings.filterSrc) {
				QString src;
				ui->plainTextEdit->appendPlainText("src: " + src.setNum(material.srcFactor, 16));
			}

			if (searchSettings.filterDst) {
				QString dst;
				ui->plainTextEdit->appendPlainText("dst: " + dst.setNum(material.dstFactor, 16));
			}

			if (searchSettings.filterPropertyName) {
				QString propertyText = "NU_" + searchSettings.materialProperty + "\n";

				// Add the space separated values to a new line.
				QList<float> values = material.properties["NU_" + searchSettings.materialProperty];
				for (int i = 0; i < values.size(); i++) {
					propertyText += QString::number(values.at(i)) + " ";
				}
				ui->plainTextEdit->appendPlainText(propertyText);
			}

			// Put a line between each material.
			ui->plainTextEdit->appendPlainText("\n");
		}
	}
}

void MainWindow::on_searchPushButton_clicked()
{
	displayMaterials();
}

void MainWindow::on_clearPushButton_clicked()
{
	ui->plainTextEdit->clear();
}

void MainWindow::on_flags1LineEdit_editingFinished()
{
	// Use hex format. Ex: 9A011063
	QString text = ui->flags1LineEdit->text();
	bool ok;
	searchSettings.flags1 = (text.toUInt(&ok, 16));
}

void MainWindow::on_flags2LineEdit_editingFinished()
{
	// Use hex format. Ex: 9A011063
	QString text = ui->flags2LineEdit->text();
	bool ok;
	searchSettings.flags2 = (text.toUInt(&ok, 16));
}

void MainWindow::on_srcLineEdit_editingFinished()
{
	// Use hex format
	QString text = ui->srcLineEdit->text();
	bool ok;
	searchSettings.srcFactor = (text.toInt(&ok, 16));
}

void MainWindow::on_dstLineEdit_editingFinished()
{
	// Use hex format
	QString text = ui->dstLineEdit->text();
	bool ok;
	searchSettings.dstFactor = (text.toInt(&ok, 16));
}

void MainWindow::on_actionAbout_triggered()
{
	// Display license information.
	QString link = "<a href='https://github.com/ScanMountGoat/Smash-Material-XML/blob/master/license.txt'>GPL License</a>";
	QMessageBox::about(0, "About", link);
}

void MainWindow::on_matPropLineEdit_editingFinished()
{
	QString text = ui->matPropLineEdit->text();
	searchSettings.materialProperty = text;
}

void MainWindow::on_matPropCheckBox_clicked()
{
	bool isChecked = ui->matPropCheckBox->isChecked();
	ui->matPropContainer->setEnabled(isChecked);
	searchSettings.filterPropertyName = isChecked;
}
