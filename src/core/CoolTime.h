/**
 *  Copyright (c) 2018 La Cool Co SAS
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 */

#ifndef COOLTIME_H
#define COOLTIME_H

#include <Arduino.h>

#include <DS1337.h>
#include <ESP8266WiFi.h>
#include <coredecls.h>
#include <time.h>

#define NTP_TIMEOUT_MS 15000
#define YEAR_2K 2000

class CoolTime {

public:
  static CoolTime &getInstance();
  void begin();
  bool sync();
  void printStatus();
  void setDateTime(int year, int month, int day, int hour, int minutes,
                   int seconds);
  String getIso8601DateTime();
  DS1337 rtc;
  static bool ntpSync;

  CoolTime(CoolTime const &) = delete;
  void operator=(CoolTime const &) = delete;

private:
  CoolTime() {}
};

#endif