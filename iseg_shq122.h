/*
 * iseg_shq122.h - ISEG SHQ GUI controller
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

#ifndef ISEG_SHQ122_H
#define ISEG_SHQ122_H

#include <iostream>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <math.h>
#include "ftdi.hpp"

namespace SuS {
   class IsegShq122 : public QThread {
      Q_OBJECT
      
      public:
         IsegShq122(QObject *parent = 0);
         ~IsegShq122();
         int sendCmd(QString);
         int sendCmdAgain();
         float currentVoltage;
         float currentCurrent;
         int deviceVoltageSlewRate;
         int deviceSerial;
         float deviceFirmware;
         float deviceMaxVoltage;
         float deviceComplianceVoltage;
         float deviceMaxCurrent;
         float deviceComplianceCurrent;
         float deviceComplianceCurrentTrip;
         int setVoltage(float);
         int setCurrentTrip(float);
         int setVoltageSlewRate(int);
         
         int queryDeviceInfo();
         int queryDeviceComplianceVoltage();
         int queryDeviceComplianceCurrent();
         int queryDeviceComplianceCurrentTrip();
         
         int queryVoltage();
         int queryCurrent();
         
         int applyChanges();
         
         int queryStatus();
         int queryStatus2();
         
      private:
         Ftdi::List::iterator it;
         void parseReply(QString);
         void run();
         QStringList cmdBuffer;
         QString currentCmdBuffer;
         QString cmdReply;
         
      signals:
         void cmdIsBusy();
         void cmdIsReady();
         void replyAvailable(QString);
         
         void measurementVoltageAvailable();
         void measurementCurrentAvailable();
         
         void deviceInfoAvailable();
         void deviceComplianceVoltageAvailable();
         void deviceComplianceCurrentAvailable();
         void deviceComplianceCurrentTripAvailable();
         void deviceVoltageSlewRateAvailable();
         
         void deviceStatusOn();
         void deviceStatusOff();
         void deviceStatusManual();
         void deviceStatusCompliance();
         void deviceStatusInhibit();
         void deviceStatusBadQuality();
         void deviceStatusLowToHigh();
         void deviceStatusHighToLow();
         void deviceStatusLookAtStatus();
         void deviceStatusSoftCompliance();
   };
}

#endif
