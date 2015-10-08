/*
 * frmmain.h - ISEG SHQ GUI controller
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

#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <ctime>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "iseg_shq122.h"

class FrmMain : public QMainWindow {
   Q_OBJECT

   public:
      FrmMain(QWidget *parent = 0);
      ~FrmMain();

   public slots:
      void btnControlApply_clicked();
      void btnControlOnOff_clicked();
      void chkLogging_stateChanged(int);
      void edtComment_editingFinished();
      void btnDirectControlEvaluate_clicked();
      void chkDirectControl_stateChanged(int);
      void hv_replyAvailable(QString);
      void hv_cmdIsBusy();
      void hv_cmdIsReady();
      void hv_measurementVoltageAvailable();
      void hv_measurementCurrentAvailable();
      void hvUpdateTimer_timeout();
      void loggingTimer_timeout();
      void hv_deviceInfoAvailable();
      void hv_deviceComplianceVoltageAvailable();
      void hv_deviceComplianceCurrentAvailable();
      void hv_deviceComplianceCurrentTripAvailable();
      void hv_deviceStatusOn();
      void hv_deviceStatusOff();
      void hv_deviceStatusManual();
      void hv_deviceStatusCompliance();
      void hv_deviceStatusInhibit();
      void hv_deviceStatusBadQuality();
      void hv_deviceStatusLowToHigh();
      void hv_deviceStatusHighToLow();
      void hv_deviceStatusLookAtStatus();
      void hv_deviceStatusSoftCompliance();
      void wTabs_currentChanged(int);
      void btnMeasStart_clicked();
      void measTimer_timeout();
      void hv_measurementVoltageAvailableForMeasurement();
      void hv_measurementCurrentAvailableForMeasurement();

    protected:
      void closeEvent(QCloseEvent *event);

    private:
      SuS::IsegShq122 *hv;
      QTabWidget *wTabs;
      QLineEdit *edtCommand;
      QTextEdit *edtDebug;
      QLabel *lblActualValuesVoltageVal;
      QLabel *lblActualValuesCurrentVal;
      QLabel *lblDeviceInfoSerialVal;
      QLabel *lblDeviceInfoFirmwareVal;
      QLabel *lblDeviceInfoMaxVoltageVal;
      QLabel *lblDeviceInfoComplianceVoltageVal;
      QLabel *lblDeviceInfoMaxCurrentVal;
      QLabel *lblDeviceInfoComplianceCurrentVal;
      QLabel *lblDeviceInfoComplianceCurrentTripVal;
      QGroupBox *grpControl;
      QDoubleSpinBox *spnControlVoltageVal;
      QDoubleSpinBox *spnControlCurrentTripVal;
      QSpinBox *spnControlVoltageSlewRateVal;
      QPushButton *btnControlApply;
      QPushButton *btnControlOnOff;
      QCheckBox *chkLogging;
      QLineEdit *edtComment;
      QString stringComment;
      std::ofstream ofstreamLogfile;
      QCheckBox *chkDirectControl;
      QGroupBox *grpDirectControl;
      QTimer *hvUpdateTimer;
      int hv_lastMeasurement;
      QTimer *loggingTimer;
      QLabel *lblStatusCommunication;
      QLabel *lblStatusDevice;
      QWidget *wTabMain;
      QWidget *wTabMeasure;
      QDoubleSpinBox *spnMeasVoltageStartVal;
      QDoubleSpinBox *spnMeasVoltageStopVal;
      QSpinBox *spnMeasStepNumVal;
      QSpinBox *spnMeasAverageNumVal;
      float measStepSize;
      double *measDataVoltage;
      double *measDataCurrent;
      int measState;
      int measStepNumber;
      int measCompleted;
      static const int measStateSetVoltage = 0;
      static const int measStateReadCurrent = 1;
      static const int measStateWaitForCompletion = 2;
      static const int measStateDone = 3;
      QwtPlotCurve *cCurve;
      QwtPlot *pMeasure;
      QPushButton *btnMeasStart;
      int measWaitCounter;
      int measAverageNumber;
};

#endif
