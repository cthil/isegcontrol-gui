/*
 * iseg_shq122.cpp - ISEG SHQ GUI controller
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

#include "iseg_shq122.h"

namespace SuS {
   IsegShq122::IsegShq122(QObject *parent) : QThread(parent) {
      int vid = 0x0403;
      int pid = 0x6001;
      
      Ftdi::List* list = Ftdi::List::find_all(vid, pid);
      for (it = list->begin(); it != list->end(); ++it) {
         int rc = it->open();
         if (rc == 0) {
            it->set_baud_rate(9600);
            it->set_line_property(BITS_8, STOP_BIT_1, NONE);
            it->set_flow_control(0); // NONE
            it->flush();
            
            if (!isRunning()) {
               start(LowPriority);
            }
            
            sendCmd(QString("W=100")); // Minimum command delay
            
            emit cmdIsReady();
				break;
         } else {
				std::cerr << "USB open failed." << std::endl;
                std::cerr << "Return code: " << rc << std::endl;
                std::cerr << "Error message: " << it->error_string() << std::endl;
			}
      }
   }
   
   IsegShq122::~IsegShq122() {
   }
   
   void IsegShq122::run() {
      int charAcked = 0;
      int cmdAcked = 1;
      unsigned char currentCharOut;
      int rc;
      unsigned char currentCharIn;
      forever {
         if ((currentCmdBuffer.isEmpty()) && (cmdAcked)) {
            if (cmdBuffer.isEmpty()) {
            } else {
               currentCmdBuffer = cmdBuffer.takeFirst();
               charAcked = 1;
               cmdAcked = 0;
            }
         }
         
         if (!currentCmdBuffer.isEmpty()) {
            if (charAcked == 1) {
               currentCharOut = *(currentCmdBuffer.left(1).toAscii());
               charAcked = 0;
            }
            it->write(&currentCharOut, 1);
         }
         
         rc = it->read(&currentCharIn, 1);
         if (rc < 0) {
            // Lesefehler
         } else if (rc > 0) {
            if (!currentCmdBuffer.isEmpty()) {
               // Ge'echo'te Zeichen
               if (currentCharIn == currentCharOut) {
                  // Richtiges Zeichen zurueck
                  charAcked = 1;
                  currentCmdBuffer.remove(0, 1);
                  cmdReply.append(currentCharIn);
                  if (currentCmdBuffer.isEmpty()) {
                  }
               } else {
                  // Falsches Zeichen zurueck
						std::cout << "Wrong readback" << std::endl;
               }
            } else {
               // Antwort
               cmdReply.append(currentCharIn);
               if (cmdReply.right(2) == "\r\n") {
                  cmdReply.resize(cmdReply.length() - 2);
                  parseReply(cmdReply);
                  emit replyAvailable(cmdReply);
                  cmdReply.clear();
                  if (cmdBuffer.isEmpty()) {
                     emit cmdIsReady();
                  }
                  cmdAcked = 1;
               }
            }
         } else {
         }
//         usleep(1000);
      }
   }
   
   int IsegShq122::sendCmd(QString cmd) {
      emit cmdIsBusy();
      cmdBuffer.append(cmd.append("\r\n"));
      return 0;
   }   
   
   void IsegShq122::parseReply(QString replyString) {
      if (replyString.split("\r\n").size() == 2) {
         QString cmd = replyString.split("\r\n")[0];
         QString reply = replyString.split("\r\n")[1];
         
         if (cmd == "#") {
            QRegExp rx("(\\d+);(\\d+\\.\\d+);(\\d+)V;(\\d+)mA");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               deviceSerial = rx.cap(1).toInt();;
               deviceFirmware = rx.cap(2).toFloat();
               deviceMaxVoltage = rx.cap(3).toFloat();
               deviceMaxCurrent = rx.cap(4).toFloat() / pow(10, 3);
               emit deviceInfoAvailable();
            }
         } else if (cmd == "U1") {
            QRegExp rx("([+-])(\\d+)([+-])(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               currentVoltage = (rx.cap(1) == "+" ? +1 : -1) * (rx.cap(2).toInt() * pow(10, (rx.cap(3) == "+" ? +1 : -1) * rx.cap(4).toInt()));
               emit measurementVoltageAvailable();
            }
         } else if (cmd == "I1") {
            QRegExp rx("(\\d+)([+-])(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               currentCurrent = (rx.cap(1).toInt() * pow(10, (rx.cap(2) == "+" ? +1 : -1) * rx.cap(3).toInt()));
               emit measurementCurrentAvailable();
            }
         } else if (cmd == "M1") {
            QRegExp rx("(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               deviceComplianceVoltage = rx.cap(1).toInt() / 100.0 * deviceMaxVoltage;
               emit deviceComplianceVoltageAvailable();
            }
         } else if (cmd == "N1") {
            QRegExp rx("(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               deviceComplianceCurrent = rx.cap(1).toInt() / 100.0 * deviceMaxCurrent;
               emit deviceComplianceCurrentAvailable();
            }
         } else if (cmd == "L1") {
            QRegExp rx("(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               deviceComplianceCurrentTrip = (rx.cap(1).toInt() / 1000.0);
               emit deviceComplianceCurrentTripAvailable();
            }
         } else if (cmd == "V1") {
            QRegExp rx("(\\d+)");
            int pos = rx.indexIn(reply);
            if (pos > -1) {
               deviceVoltageSlewRate = rx.cap(1).toInt();
               emit deviceVoltageSlewRateAvailable();
            }
         } else if (cmd == "S1") {
            reply = reply.section('=', 1);
            if (reply == "ON") {
               emit deviceStatusOn();
            } else if (reply == "OFF") {
               emit deviceStatusOff();
            } else if (reply == "MAN") {
               emit deviceStatusManual();
            } else if (reply == "ERR") {
               emit deviceStatusCompliance();
            } else if (reply == "INH") {
               emit deviceStatusInhibit();
            } else if (reply == "QUA") {
               emit deviceStatusBadQuality();
            } else if (reply == "L2H") {
               emit deviceStatusLowToHigh();
            } else if (reply == "H2L") {
               emit deviceStatusHighToLow();
            } else if (reply == "LAS") {
               emit deviceStatusLookAtStatus();
            } else if (reply == "TRP") {
               emit deviceStatusSoftCompliance();
            } else {
            }
         } else if (cmd == "T1") {
         } else {
         }
      } else {
      }
   }
   
   int IsegShq122::queryDeviceInfo() {
      return sendCmd(QString("#"));
   }
   
   int IsegShq122::queryDeviceComplianceVoltage() {
      return sendCmd(QString("M1"));
   }
   
   int IsegShq122::queryDeviceComplianceCurrent() {
      return sendCmd(QString("N1"));
   }

   int IsegShq122::queryDeviceComplianceCurrentTrip() {
      return sendCmd(QString("L1"));
   }

   int IsegShq122::queryVoltage() {
      return sendCmd(QString("U1"));
   }
   
   int IsegShq122::queryCurrent() {
      return sendCmd(QString("I1"));
   }
      
   int IsegShq122::queryStatus() {
      return sendCmd(QString("S1"));
   }
   
   int IsegShq122::queryStatus2() {
      return sendCmd(QString("T1"));
   }
   
   int IsegShq122::setVoltage(float voltage) {
      if ((voltage >= 0.0) && (voltage <= deviceComplianceVoltage)) {
         return sendCmd(QString("D1=%1").arg(QString::number(voltage, 'f', 2).rightJustified(7, '0')));
      } else {
         return 1;
      }
   }
   
   int IsegShq122::setCurrentTrip(float current) {
      if (current == 0) {
         return sendCmd(QString("L1=0"));
      } else if (current < 1e-3) {
         return sendCmd(QString("LS1=%1").arg(current * pow(10, 6)));
      } else {
         return sendCmd(QString("LB1=%1").arg(current * pow(10, 3)));
      }
   }
   
   int IsegShq122::setVoltageSlewRate(int slewRate) {
      return sendCmd(QString("V1=%1").arg(slewRate));
   }
   
   int IsegShq122::applyChanges() {
      return sendCmd(QString("G1"));
   }
}
