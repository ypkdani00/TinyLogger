#pragma once

#ifndef TINYLOGGER_NTPCLIENT
  #define TINYLOGGER_NTPCLIENT 0
#endif

#include <vector>
#include <Arduino.h>
#if TINYLOGGER_NTPCLIENT
  #include <NTPClient.h>
#endif

class TinyLogger {
public:
  enum Level {
    SILENT,
    FATAL,
    ERROR,
    WARNING,
    INFO,
    NOTICE,
    TRACE,
    VERBOSE
  };

  void begin(Stream* stream, Level level = Level::ERROR) {
    this->streams.push_back(stream);
    this->level = level;
  }

  void addStream(Stream* stream) {
    this->streams.push_back(stream);
  }

  void clearStreams() {
    this->streams.clear();
  }

  Level getLevel() {
    return this->level;
  }

  void setLevel(Level level) {
    this->level = level;
  }

  void setDate(struct tm* date) {
    this->date = date;
  }

  void setDate() {
    this->date = nullptr;
  }

  bool isDateSet() {
    return this->date != nullptr;
  }

#if TINYLOGGER_NTPCLIENT
  void setNtpClient(NTPClient* ntpClient) {
    this->ntpClient = ntpClient;
  }

  void setNtpClient() {
    this->ntpClient = nullptr;
  }

  bool isNtpClientSet() {
    return this->ntpClient != nullptr;
  }
#endif

  const char* getServiceTemplate() {
    return this->serviceTemplate;
  }

  void setServiceTemplate(const char* value) {
    this->serviceTemplate = value;
  }

  const char* getServiceDelimiter() {
    return this->serviceDelim;
  }

  void setServiceDelimiter(const char* value) {
    this->serviceTemplate = value;
  }

  void setServiceDelimiter() {
    this->serviceDelim = nullptr;
  }

  void setLevelTemplate(const char* value) {
    this->levelTemplate = value;
  }

  const char* getLevelTemplate() {
    return this->levelTemplate;
  }

  template <class T> void print(T msg) {
    for (Stream* stream : this->streams) {
      stream->print(msg);
    }
  }

  template <class T> void println(T msg) {
    for (Stream* stream : this->streams) {
      stream->println(msg);
    }
  }

  template <class T, typename... Args> void printf(T msg, Args... args) {
    for (Stream* stream : this->streams) {
      stream->printf(msg, args...);
    }
  }

  template <class T, typename... Args> void printFormatted(Level level, const char* service, bool nl, T msg, Args... args) {
    if (level > this->level) {
      return;
    }

    if (this->dateTemplate != nullptr) {
      struct tm* tm = nullptr;
      if (this->isDateSet()) {
        tm = this->date;
      }

#if TINYLOGGER_NTPCLIENT
      if (tm == nullptr && this->isNtpClientSet()) {
        time_t now = this->ntpClient->getEpochTime();
        tm = localtime(&now);
      }
#endif

      if (tm != nullptr) {
        char* buffer = new char[64];
        if (strftime(buffer, sizeof(buffer), this->dateTemplate, tm) != 0) {
          this->print(buffer);
        }
        delete[] buffer;
      }
    }

    if (service != nullptr && strlen(service) > 0) {
      if (this->serviceDelim != nullptr && strlen(this->serviceDelim) > 0 && strstr(service, this->serviceDelim) != NULL) {
        char* tmp = strdup(service);
        char* item = strtok(tmp, this->serviceDelim);
        
        while (item != NULL) {
          this->printf(this->serviceTemplate, item);
          item = strtok(NULL, this->serviceDelim);
        }
        free(tmp);

      } else {
        this->printf(this->serviceTemplate, service);
      }
    }
    
    this->printf(this->levelTemplate, TinyLogger::level2str(level));
    this->print(" ");
    this->printf(msg, args...);

    if (nl) {
      this->print(this->nlChar);
    }
  }

  template <class T, typename... Args> void fatal(T msg, Args... args) {
    this->printFormatted(Level::FATAL, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void sfatal(const char* service, T msg, Args... args) {
    this->printFormatted(Level::FATAL, service, false, msg, args...);
  }

  template <class T, typename... Args> void fatalln(T msg, Args... args) {
    this->printFormatted(Level::FATAL, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void sfatalln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::FATAL, service, true, msg, args...);
  }


  template <class T, typename... Args> void error(T msg, Args... args) {
    this->printFormatted(Level::ERROR, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void serror(const char* service, T msg, Args... args) {
    this->printFormatted(Level::ERROR, service, false, msg, args...);
  }

  template <class T, typename... Args> void errorln(T msg, Args... args) {
    this->printFormatted(Level::ERROR, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void serrorln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::ERROR, service, true, msg, args...);
  }


  template <class T, typename... Args> void warning(T msg, Args... args) {
    this->printFormatted(Level::WARNING, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void swarning(const char* service, T msg, Args... args) {
    this->printFormatted(Level::WARNING, service, false, msg, args...);
  }

  template <class T, typename... Args> void warningln(T msg, Args... args) {
    this->printFormatted(Level::WARNING, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void swarningln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::WARNING, service, true, msg, args...);
  }


  template <class T, typename... Args> void info(T msg, Args... args) {
    this->printFormatted(Level::INFO, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void sinfo(const char* service, T msg, Args... args) {
    this->printFormatted(Level::INFO, service, false, msg, args...);
  }

  template <class T, typename... Args> void infoln(T msg, Args... args) {
    this->printFormatted(Level::INFO, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void sinfoln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::INFO, service, true, msg, args...);
  }


  template <class T, typename... Args> void notice(T msg, Args... args) {
    this->printFormatted(Level::NOTICE, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void snotice(const char* service, T msg, Args... args) {
    this->printFormatted(Level::NOTICE, service, false, msg, args...);
  }

  template <class T, typename... Args> void noticeln(T msg, Args... args) {
    this->printFormatted(Level::NOTICE, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void snoticeln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::NOTICE, service, true, msg, args...);
  }


  template <class T, typename... Args> void trace(T msg, Args... args) {
    this->printFormatted(Level::TRACE, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void strace(const char* service, T msg, Args... args) {
    this->printFormatted(Level::TRACE, service, false, msg, args...);
  }

  template <class T, typename... Args> void traceln(T msg, Args... args) {
    this->printFormatted(Level::TRACE, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void straceln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::TRACE, service, true, msg, args...);
  }


  template <class T, typename... Args> void verbose(T msg, Args... args) {
    this->printFormatted(Level::VERBOSE, nullptr, false, msg, args...);
  }

  template <class T, typename... Args> void sverbose(const char* service, T msg, Args... args) {
    this->printFormatted(Level::VERBOSE, service, false, msg, args...);
  }

  template <class T, typename... Args> void verboseln(T msg, Args... args) {
    this->printFormatted(Level::VERBOSE, nullptr, true, msg, args...);
  }

  template <class T, typename... Args> void sverboseln(const char* service, T msg, Args... args) {
    this->printFormatted(Level::VERBOSE, service, true, msg, args...);
  }

  static String level2str(Level level, const char* prefix = nullptr, const char* suffix = nullptr) {
    const char* str;

    switch (level) {
      default:
      case Level::SILENT:
        str = "SILENT";
        break;
      case Level::FATAL:
        str = "FATAL";
        break;
      case Level::ERROR:
        str = "ERROR";
        break;
      case Level::WARNING:
        str = "WARN";
        break;
      case Level::INFO:
        str = "INFO";
        break;
      case Level::TRACE:
        str = "TRACE";
        break;
      case Level::VERBOSE:
        str = "VERB";
        break;
    }

    return str;
  }

protected:
  std::vector<Stream*> streams;
  Level level = Level::ERROR;
  struct tm* date = nullptr;
#if TINYLOGGER_NTPCLIENT
  NTPClient* ntpClient = nullptr;
#endif
  const char* dateTemplate = "[%d.%m.%Y %H:%M:%S]";
  const char* serviceTemplate = "[%s]";
  const char* serviceDelim = ".";
  const char* levelTemplate = "[%s]";
  const char* nlChar = "\r\n";
};

TinyLogger Log = TinyLogger();
extern TinyLogger Log;