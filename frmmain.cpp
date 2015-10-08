/*
 * frmmain.cpp - ISEG SHQ GUI controller
 *
 * Copyright (c) xxxx-2015 Christophe Thil <christophe@thil.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QtGui>

#include "frmmain.h"
#include <iostream>

FrmMain::FrmMain(QWidget *parent) : QMainWindow(parent){

   wTabs = new QTabWidget;
   setCentralWidget(wTabs);

   wTabMain = new QWidget;
   wTabMeasure = new QWidget;
   
   wTabs->addTab(wTabMain, QString(tr("Main")));
   wTabs->addTab(wTabMeasure, QString(tr("Measure")));
   wTabMeasure->setEnabled(false);
   
   // --- Main Tab ---
   
   QGroupBox *grpDeviceInfo = new QGroupBox(tr("Device Information"));
   QGridLayout *layDeviceInfo = new QGridLayout;   
   
   QLabel *lblDeviceInfoSerialDesc = new QLabel(tr("Serial Number"));
   lblDeviceInfoSerialVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoFirmwareDesc = new QLabel(tr("Firmware Version"));
   lblDeviceInfoFirmwareVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoMaxVoltageDesc = new QLabel(tr("Maximum Voltage"));
   lblDeviceInfoMaxVoltageVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoComplianceVoltageDesc = new QLabel(tr("Compliance Voltage (HW)"));
   lblDeviceInfoComplianceVoltageVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoMaxCurrentDesc = new QLabel(tr("Maximum Current"));
   lblDeviceInfoMaxCurrentVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoComplianceCurrentDesc = new QLabel(tr("Compliance Current (HW)"));
   lblDeviceInfoComplianceCurrentVal = new QLabel(tr("---"));
   QLabel *lblDeviceInfoComplianceCurrentTripDesc = new QLabel(tr("Compliance Current (SW/Trip)"));
   lblDeviceInfoComplianceCurrentTripVal = new QLabel(tr("---"));


   layDeviceInfo->addWidget(lblDeviceInfoSerialDesc, 0, 0);
   layDeviceInfo->addWidget(lblDeviceInfoSerialVal, 0, 1);
   layDeviceInfo->addWidget(lblDeviceInfoFirmwareDesc, 1, 0);
   layDeviceInfo->addWidget(lblDeviceInfoFirmwareVal, 1, 1);
   layDeviceInfo->addWidget(lblDeviceInfoMaxVoltageDesc, 2, 0);
   layDeviceInfo->addWidget(lblDeviceInfoMaxVoltageVal, 2, 1);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceVoltageDesc, 3, 0);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceVoltageVal, 3, 1);
   layDeviceInfo->addWidget(lblDeviceInfoMaxCurrentDesc, 4, 0);
   layDeviceInfo->addWidget(lblDeviceInfoMaxCurrentVal, 4, 1);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceCurrentDesc, 5, 0);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceCurrentVal, 5, 1);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceCurrentTripDesc, 6, 0);
   layDeviceInfo->addWidget(lblDeviceInfoComplianceCurrentTripVal, 6, 1);
   
   grpDeviceInfo->setLayout(layDeviceInfo);
   
   QGroupBox *grpActualValues = new QGroupBox(tr("Actual Values"));
   QGridLayout *layActualValues = new QGridLayout;
   
   QLabel *lblActualValuesVoltageDesc = new QLabel(tr("Voltage"));
   lblActualValuesVoltageVal = new QLabel(tr("---"));
   QLabel *lblActualValuesCurrentDesc = new QLabel(tr("Current"));
   lblActualValuesCurrentVal = new QLabel(tr("---"));
   
   layActualValues->addWidget(lblActualValuesVoltageDesc, 0, 0);
   layActualValues->addWidget(lblActualValuesVoltageVal, 0, 1);
   layActualValues->addWidget(lblActualValuesCurrentDesc, 1, 0);
   layActualValues->addWidget(lblActualValuesCurrentVal, 1, 1);
   
   grpActualValues->setLayout(layActualValues);
   
   grpControl = new QGroupBox(tr("Control"));
   QGridLayout *layControl = new QGridLayout;
   
   QLabel *lblControlVoltageDesc = new QLabel(tr("Voltage"));
   spnControlVoltageVal = new QDoubleSpinBox;
   spnControlVoltageVal->setMinimum(0.0);
   spnControlVoltageVal->setMaximum(0.0);
   spnControlVoltageVal->setDecimals(1);
   spnControlVoltageVal->setSuffix(tr(" V"));
   QLabel *lblControlCurrentDesc = new QLabel(tr("Current Trip"));
   spnControlCurrentTripVal = new QDoubleSpinBox;
   spnControlCurrentTripVal->setMinimum(0.0);
   spnControlCurrentTripVal->setMaximum(0.0);
   spnControlCurrentTripVal->setDecimals(4);
   spnControlCurrentTripVal->setSpecialValueText(tr("Use HW only"));
   spnControlCurrentTripVal->setSuffix(tr(" uA"));
   QLabel *lblControlVoltageSlewRateDesc = new QLabel(tr("Voltage Slew Rate"));
   spnControlVoltageSlewRateVal = new QSpinBox;
   spnControlVoltageSlewRateVal->setMinimum(0);
   spnControlVoltageSlewRateVal->setMaximum(255);
   spnControlVoltageSlewRateVal->setSuffix(tr(" V/s"));
   spnControlVoltageSlewRateVal->setValue(100);
   btnControlApply = new QPushButton(tr("Apply"));
   btnControlApply->setDefault(true);
   connect(btnControlApply, SIGNAL(clicked()), this, SLOT(btnControlApply_clicked()));
   btnControlOnOff = new QPushButton(tr("HV Soft Off"));
   connect(btnControlOnOff, SIGNAL(clicked()), this, SLOT(btnControlOnOff_clicked()));
   
   layControl->addWidget(lblControlVoltageDesc, 0, 0);
   layControl->addWidget(spnControlVoltageVal, 0, 1);
   layControl->addWidget(lblControlCurrentDesc, 1, 0);
   layControl->addWidget(spnControlCurrentTripVal, 1, 1);
   layControl->addWidget(lblControlVoltageSlewRateDesc, 2, 0);
   layControl->addWidget(spnControlVoltageSlewRateVal, 2, 1);
   layControl->addWidget(btnControlApply, 3, 1);
   layControl->addWidget(btnControlOnOff, 4, 1);

   
   grpControl->setLayout(layControl);

   grpDirectControl = new QGroupBox(tr("Direct control"));
   QVBoxLayout *layDirectControl = new QVBoxLayout;
   
   QWidget *wDirectControlButtons = new QWidget;
   QHBoxLayout *layDirectControlButtons = new QHBoxLayout;
   
   QPushButton *btnDirectControlEvaluate = new QPushButton(tr("Evaluate"));
   connect(btnDirectControlEvaluate, SIGNAL(clicked()), this, SLOT(btnDirectControlEvaluate_clicked()));

   edtCommand = new QLineEdit;
   
   layDirectControlButtons->addWidget(edtCommand);
   layDirectControlButtons->addWidget(btnDirectControlEvaluate);
   wDirectControlButtons->setLayout(layDirectControlButtons);
   
   edtDebug = new QTextEdit;
   
   layDirectControl->addWidget(wDirectControlButtons);
   layDirectControl->addWidget(edtDebug);
   
   grpDirectControl->setLayout(layDirectControl);
   grpDirectControl->setVisible(false);
   
   chkDirectControl = new QCheckBox(tr("Direct control"));
   connect(chkDirectControl, SIGNAL(stateChanged(int)), this, SLOT(chkDirectControl_stateChanged(int)));

   chkLogging = new QCheckBox(tr("Logging"));
   connect(chkLogging, SIGNAL(stateChanged(int)), this, SLOT(chkLogging_stateChanged(int)));

   edtComment = new QLineEdit;
   edtComment->setReadOnly(true);
   connect(edtComment, SIGNAL(editingFinished()), this, SLOT(edtComment_editingFinished()));

   QVBoxLayout *layMain = new QVBoxLayout;

   layMain->addWidget(grpDeviceInfo);
   layMain->addWidget(grpActualValues);
   layMain->addWidget(grpControl);
   layMain->addWidget(chkLogging);
   layMain->addWidget(edtComment);
   layMain->addWidget(chkDirectControl);
   layMain->addWidget(grpDirectControl);
   wTabMain->setLayout(layMain);
   
   // --- Measure Tab ---
   
   QVBoxLayout *layMeasure = new QVBoxLayout;
   QWidget *wMeasControl = new QWidget;
   QGridLayout *layMeasControl = new QGridLayout;
   wMeasControl->setLayout(layMeasControl);
   
   pMeasure = new QwtPlot(parent);
   cCurve = new QwtPlotCurve("Curve");
   
   cCurve->attach(pMeasure);
      
   QLabel *lblMeasVoltageStartDesc = new QLabel(tr("Start Voltage"));
   spnMeasVoltageStartVal = new QDoubleSpinBox;
   spnMeasVoltageStartVal->setMinimum(0.0);
   spnMeasVoltageStartVal->setDecimals(1);
   spnMeasVoltageStartVal->setSuffix(tr(" V"));
   QLabel *lblMeasVoltageStopDesc = new QLabel(tr("Stop Voltage"));
   spnMeasVoltageStopVal = new QDoubleSpinBox;
   spnMeasVoltageStopVal->setMinimum(0.0);
   spnMeasVoltageStopVal->setDecimals(1);
   spnMeasVoltageStopVal->setSuffix(tr(" V"));
   QLabel *lblMeasStepNumDesc = new QLabel(tr("Number of steps"));
   spnMeasStepNumVal = new QSpinBox;
   spnMeasStepNumVal->setMinimum(2);
   spnMeasStepNumVal->setMaximum(10000);
   QLabel *lblMeasAverageNumDesc = new QLabel(tr("Samples per step"));
   spnMeasAverageNumVal = new QSpinBox;
   spnMeasAverageNumVal->setMinimum(1);
   spnMeasAverageNumVal->setMaximum(10000);
   
   btnMeasStart = new QPushButton(tr("Start"));
   connect(btnMeasStart, SIGNAL(clicked()), this, SLOT(btnMeasStart_clicked()));


   layMeasControl->addWidget(lblMeasVoltageStartDesc, 0, 0);
   layMeasControl->addWidget(spnMeasVoltageStartVal, 0, 1);
   layMeasControl->addWidget(lblMeasVoltageStopDesc, 1, 0);
   layMeasControl->addWidget(spnMeasVoltageStopVal, 1, 1);
   layMeasControl->addWidget(lblMeasStepNumDesc, 2, 0);
   layMeasControl->addWidget(spnMeasStepNumVal, 2, 1);
   layMeasControl->addWidget(lblMeasAverageNumDesc, 3, 0);
   layMeasControl->addWidget(spnMeasAverageNumVal, 3, 1);
   layMeasControl->addWidget(btnMeasStart, 4, 1);

   layMeasure->addWidget(pMeasure);
   layMeasure->addWidget(wMeasControl);
      
   
   wTabMeasure->setLayout(layMeasure);   
   
   // --- End Tabs ---
   
   lblStatusCommunication = new QLabel;
   lblStatusDevice = new QLabel;
   
   statusBar()->addPermanentWidget(lblStatusCommunication, 5);
   statusBar()->addPermanentWidget(lblStatusDevice, 5);
   
   lblStatusCommunication->setText(QString(tr("Initializing")));
   lblStatusDevice->setText(QString(tr("---")));

   setWindowTitle(tr("IsegHV Control"));

   hv = new SuS::IsegShq122();
   connect(hv, SIGNAL(replyAvailable(QString)), this, SLOT(hv_replyAvailable(QString)));
   connect(hv, SIGNAL(cmdIsBusy()), this, SLOT(hv_cmdIsBusy()));
   connect(hv, SIGNAL(cmdIsReady()), this, SLOT(hv_cmdIsReady()));
   
   connect(hv, SIGNAL(measurementVoltageAvailable()), this, SLOT(hv_measurementVoltageAvailable()));
   connect(hv, SIGNAL(measurementCurrentAvailable()), this, SLOT(hv_measurementCurrentAvailable()));
   
   connect(hv, SIGNAL(deviceInfoAvailable()), this, SLOT(hv_deviceInfoAvailable()));
   connect(hv, SIGNAL(deviceComplianceVoltageAvailable()), this, SLOT(hv_deviceComplianceVoltageAvailable()));
   connect(hv, SIGNAL(deviceComplianceCurrentAvailable()), this, SLOT(hv_deviceComplianceCurrentAvailable()));
   connect(hv, SIGNAL(deviceComplianceCurrentTripAvailable()), this, SLOT(hv_deviceComplianceCurrentTripAvailable()));
   
   connect(hv, SIGNAL(deviceStatusOn()), this, SLOT(hv_deviceStatusOn()));
   connect(hv, SIGNAL(deviceStatusOff()), this, SLOT(hv_deviceStatusOff()));
   connect(hv, SIGNAL(deviceStatusManual()), this, SLOT(hv_deviceStatusManual()));
   connect(hv, SIGNAL(deviceStatusCompliance()), this, SLOT(hv_deviceStatusCompliance()));
   connect(hv, SIGNAL(deviceStatusInhibit()), this, SLOT(hv_deviceStatusInhibit()));
   connect(hv, SIGNAL(deviceStatusBadQuality()), this, SLOT(hv_deviceStatusBadQuality()));
   connect(hv, SIGNAL(deviceStatusLowToHigh()), this, SLOT(hv_deviceStatusLowToHigh()));
   connect(hv, SIGNAL(deviceStatusHighToLow()), this, SLOT(hv_deviceStatusHighToLow()));
   connect(hv, SIGNAL(deviceStatusLookAtStatus()), this, SLOT(hv_deviceStatusLookAtStatus()));
   connect(hv, SIGNAL(deviceStatusSoftCompliance()), this, SLOT(hv_deviceStatusSoftCompliance()));
   
   hv_lastMeasurement = 0;
   hvUpdateTimer = new QTimer(this);
   connect(hvUpdateTimer, SIGNAL(timeout()), this, SLOT(hvUpdateTimer_timeout()));
   hvUpdateTimer->start(1000);

   loggingTimer = new QTimer(this);
   connect(loggingTimer, SIGNAL(timeout()), this, SLOT(loggingTimer_timeout()));
   
   connect(wTabs, SIGNAL(currentChanged(int)), this, SLOT(wTabs_currentChanged(int)));
}

FrmMain::~FrmMain() {
}

void FrmMain::closeEvent(QCloseEvent *event) {
   if (ofstreamLogfile.is_open()) {
      ofstreamLogfile.close();
   }
   event->accept();
}

void FrmMain::btnDirectControlEvaluate_clicked() {
   hv->sendCmd(edtCommand->text());
   edtCommand->clear();
}

void FrmMain::hv_replyAvailable(QString reply) {
   edtDebug->append(reply);
}

void FrmMain::hv_cmdIsBusy() {
   lblStatusCommunication->setText(QString(tr("Comm busy")));
   hvUpdateTimer->stop();
}

void FrmMain::hv_cmdIsReady() {
   lblStatusCommunication->setText(QString(tr("Comm ready")));
   if (wTabs->currentIndex() == wTabs->indexOf(wTabMain)) {
      hvUpdateTimer->start();
   }
}

void FrmMain::hv_measurementVoltageAvailable() {
   lblActualValuesVoltageVal->setText(QString("%1 V").arg(hv->currentVoltage));
}

void FrmMain::hv_measurementCurrentAvailable() {
   lblActualValuesCurrentVal->setText(QString("%1 uA").arg(hv->currentCurrent * pow(10, 6)));
}

void FrmMain::hvUpdateTimer_timeout() {
   if (hv_lastMeasurement == 0) {
      hv->queryDeviceInfo();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 1) {
      hv->queryDeviceComplianceVoltage();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 2) {
      hv->queryDeviceComplianceCurrent();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 3) {
      hv->queryDeviceComplianceCurrentTrip();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 4) {
      hv->queryVoltage();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 5) {
      hv->queryCurrent();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 6) {
      hv->queryStatus();
      hv_lastMeasurement++;
   } else if (hv_lastMeasurement == 7) {
      hv->queryStatus2();
      wTabMeasure->setEnabled(true);
      hv_lastMeasurement = 1;
   } else {
      hv_lastMeasurement = 0;
   }
}

void FrmMain::loggingTimer_timeout() {
    time_t timer;
    struct tm *timeinfo;
    time(&timer);
    timeinfo = localtime(&timer);
    char buffer[20];
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", timeinfo);

    ofstreamLogfile << buffer << " " << hv->currentVoltage << " " << hv->currentCurrent << " " << stringComment.toStdString() << std::endl;
    stringComment.clear();
}

void FrmMain::hv_deviceInfoAvailable() {
   lblDeviceInfoSerialVal->setText(QString("%1").arg(hv->deviceSerial));
   lblDeviceInfoFirmwareVal->setText(QString("%1").arg(hv->deviceFirmware));
   lblDeviceInfoMaxVoltageVal->setText(QString("%1 V").arg(hv->deviceMaxVoltage));
   lblDeviceInfoMaxCurrentVal->setText(QString("%1 uA").arg(hv->deviceMaxCurrent * pow(10, 6)));
}

void FrmMain::hv_deviceComplianceVoltageAvailable() {
   lblDeviceInfoComplianceVoltageVal->setText(QString("%1 V").arg(hv->deviceComplianceVoltage));
   spnControlVoltageVal->setMaximum(hv->deviceComplianceVoltage);
   spnMeasVoltageStartVal->setMaximum(hv->deviceComplianceVoltage);
   spnMeasVoltageStopVal->setMaximum(hv->deviceComplianceVoltage);
}

void FrmMain::hv_deviceComplianceCurrentAvailable() {
   lblDeviceInfoComplianceCurrentVal->setText(QString("%1 uA").arg(hv->deviceComplianceCurrent * pow(10, 6)));
   spnControlCurrentTripVal->setMaximum(hv->deviceComplianceCurrent * pow(10, 6));
}

void FrmMain::hv_deviceComplianceCurrentTripAvailable() {
   if (hv->deviceComplianceCurrentTrip == 0) {
      lblDeviceInfoComplianceCurrentTripVal->setText(QString(tr("Use HW only")));
   } else {
      lblDeviceInfoComplianceCurrentTripVal->setText(QString("%1 uA").arg(hv->deviceComplianceCurrentTrip * pow(10, 6)));
   }
}

void FrmMain::chkLogging_stateChanged(int state) {
    if (state) {
        QString stringLoggingFilename = QFileDialog::getSaveFileName(this, QString(tr("Select file for logging")), "", QString(tr("Log files (*.log);;Text files (*.txt)")), 0, QFileDialog::DontConfirmOverwrite);
        if (stringLoggingFilename.isNull() == false) {
            ofstreamLogfile.open(stringLoggingFilename.toStdString().c_str(), std::ios::app);
            ofstreamLogfile << "# Timestamp Voltage (V) Current (A) Comment" << std::endl;
            edtComment->setReadOnly(false);
            loggingTimer->start(1000);
        } else {
            chkLogging->setChecked(false);
        }
    } else {
        edtComment->setReadOnly(true);
        loggingTimer->stop();
        ofstreamLogfile.close();
    }
}

void FrmMain::edtComment_editingFinished() {
    stringComment = edtComment->text();
    edtComment->clear();
}

void FrmMain::chkDirectControl_stateChanged(int state) {
   grpDirectControl->setVisible(chkDirectControl->isChecked());
}

void FrmMain::btnControlApply_clicked() {
   hv->setVoltage(spnControlVoltageVal->value());
   hv->setCurrentTrip(spnControlCurrentTripVal->value()/pow(10,6));
   hv->setVoltageSlewRate(spnControlVoltageSlewRateVal->value());
   hv->applyChanges();
}

void FrmMain::btnControlOnOff_clicked() {
	// FIXME Should save HV state other than using the button text as state indicator
   if (btnControlOnOff->text() == QString(tr("HV Soft On"))) {
      btnControlOnOff->setText(QString(tr("HV Soft Off")));
      hv->setVoltage(spnControlVoltageVal->value());
      hv->setVoltageSlewRate(spnControlVoltageSlewRateVal->value());
      hv->applyChanges();
   } else {
      btnControlOnOff->setText(QString(tr("HV Soft On")));
      hv->setVoltage(0);
      hv->setVoltageSlewRate(255);
      hv->applyChanges();
   }
}

void FrmMain::hv_deviceStatusOn() {
   lblStatusDevice->setText(QString(tr("HV on")));
}
void FrmMain::hv_deviceStatusOff() {
   lblStatusDevice->setText(QString(tr("HV off")));
}
void FrmMain::hv_deviceStatusManual() {
   lblStatusDevice->setText(QString(tr("HV manual")));
}
void FrmMain::hv_deviceStatusCompliance() {
   lblStatusDevice->setText(QString(tr("HV compliance")));
}
void FrmMain::hv_deviceStatusInhibit() {
   lblStatusDevice->setText(QString(tr("HV inhibit")));
}
void FrmMain::hv_deviceStatusBadQuality() {
   lblStatusDevice->setText(QString(tr("HV bad quality")));
}
void FrmMain::hv_deviceStatusLowToHigh() {
   lblStatusDevice->setText(QString(tr("HV rising")));
}
void FrmMain::hv_deviceStatusHighToLow() {
   lblStatusDevice->setText(QString(tr("HV falling")));
}
void FrmMain::hv_deviceStatusLookAtStatus() {
   lblStatusDevice->setText(QString(tr("HV look at status")));
}
void FrmMain::hv_deviceStatusSoftCompliance() {
   lblStatusDevice->setText(QString(tr("HV soft compliance")));
}

void FrmMain::wTabs_currentChanged(int index) {
   if (index == wTabs->indexOf(wTabMain)) {
      hvUpdateTimer->start();
   } else if (index == wTabs->indexOf(wTabMeasure)) {
      hvUpdateTimer->stop();
   } else {
   }
}

void FrmMain::btnMeasStart_clicked() {
   btnMeasStart->setEnabled(false);
   measStepSize = (spnMeasVoltageStopVal->value() - spnMeasVoltageStartVal->value()) / (spnMeasStepNumVal->value() - 1);
   measDataVoltage = (double*) malloc(sizeof(double) * spnMeasStepNumVal->value());
   measDataCurrent = (double*) malloc(sizeof(double) * spnMeasStepNumVal->value());
   measState = measStateSetVoltage;
   measStepNumber = 0;
   measCompleted = 0;
   connect(hv, SIGNAL(measurementVoltageAvailable()), this, SLOT(hv_measurementVoltageAvailableForMeasurement()));
   connect(hv, SIGNAL(measurementCurrentAvailable()), this, SLOT(hv_measurementCurrentAvailableForMeasurement()));
   QTimer::singleShot(2000, this, SLOT(measTimer_timeout()));

}

void FrmMain::measTimer_timeout() {
   if (measState == measStateSetVoltage) {
      measCompleted = 0;
      measAverageNumber = 0;
      hv->setVoltage(spnMeasVoltageStartVal->value() + (measStepSize * measStepNumber));
      hv->applyChanges();
      hv->queryStatus();
      measState = measStateReadCurrent;
      QTimer::singleShot(5000, this, SLOT(measTimer_timeout()));
   } else if (measState == measStateReadCurrent) {
      hv->queryVoltage();
      hv->queryCurrent();
      measWaitCounter = 0;
      measState = measStateWaitForCompletion;
      QTimer::singleShot(1000, this, SLOT(measTimer_timeout()));
   } else if (measState == measStateWaitForCompletion) {
      if (measCompleted == ((1 << 0) | (1 << 1))) {
         if (measStepNumber >= (spnMeasStepNumVal->value() - 1)) {
            measState = measStateDone;
         } else {
            cCurve->setData(measDataVoltage, measDataCurrent, measStepNumber+1);
            pMeasure->replot();
            if (measAverageNumber >= (spnMeasAverageNumVal->value() - 1)) {
               measState = measStateSetVoltage;
               measStepNumber++;
            } else {
               measCompleted = 0;
               measState = measStateReadCurrent;
               measAverageNumber++;
            }
         }
      }
      if (measWaitCounter >= 10) {
         measState = measStateReadCurrent;
      }
      measWaitCounter++;
      QTimer::singleShot(1000, this, SLOT(measTimer_timeout()));
   } else if (measState == measStateDone) {
      disconnect(hv, SIGNAL(measurementVoltageAvailable()), this, SLOT(hv_measurementVoltageAvailableForMeasurement()));
      disconnect(hv, SIGNAL(measurementCurrentAvailable()), this, SLOT(hv_measurementCurrentAvailableForMeasurement()));
      cCurve->setData(measDataVoltage, measDataCurrent, spnMeasStepNumVal->value());
      pMeasure->replot();
      hv->setVoltage(0.0);
      hv->applyChanges();
      
      btnMeasStart->setEnabled(true);
      
      for (int i=0; i < spnMeasStepNumVal->value(); i++) {
         std::cout << measDataVoltage[i] << ":" << measDataCurrent[i] << std::endl;
      }
      
   } else {
   }
}

void FrmMain::hv_measurementVoltageAvailableForMeasurement() {
   if (measAverageNumber == 0.0) {
      measDataVoltage[measStepNumber] = hv->currentVoltage;
   } else {
      measDataVoltage[measStepNumber] = (measDataVoltage[measStepNumber] + hv->currentVoltage) / 2.0;
   }
   measCompleted |= (1 << 0);
}

void FrmMain::hv_measurementCurrentAvailableForMeasurement() {
   if (measAverageNumber == 0) {
      measDataCurrent[measStepNumber] = hv->currentCurrent;
   } else {
      measDataCurrent[measStepNumber] = (measDataCurrent[measStepNumber] + hv->currentCurrent) / 2.0;
   }
   measCompleted |= (1 << 1);
}
