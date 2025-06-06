#pragma once
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

class SerialPort {
public:
    SerialPort(const std::string& device = "/dev/ttyACM0", speed_t baudRate = B9600)
        : devicePath(device), baudRate(baudRate), serialFd(-1) {}

    ~SerialPort() {
        if (serialFd != -1) {
            close(serialFd);
        }
    }

    bool init() {
        serialFd = open(devicePath.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (serialFd < 0) {
            std::cerr << "Error opening " << devicePath << ": " << strerror(errno) << std::endl;
            return false;
        }

        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(serialFd, &tty) != 0) {
            std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
            return false;
        }

        cfsetospeed(&tty, baudRate);
        cfsetispeed(&tty, baudRate);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
        tty.c_iflag &= ~IGNBRK;
        tty.c_lflag = 0;
        tty.c_oflag = 0;
        tty.c_cc[VMIN]  = 1;
        tty.c_cc[VTIME] = 1;

        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~(PARENB | PARODD);
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr(serialFd, TCSANOW, &tty) != 0) {
            std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

    void send(const std::string& data) {
        writeString(data);
    }


    std::string readString() {
        if (serialFd == -1) return "";

        char buf[256];
        ssize_t n = read(serialFd, buf, sizeof(buf));
        if (n > 0) {
            readBuffer.append(buf, n);

            // Repeatedly extract messages, keeping only the last one
            std::string lastMessage;
            size_t newlinePos;
            while ((newlinePos = readBuffer.find('\n')) != std::string::npos) {
                lastMessage = readBuffer.substr(0, newlinePos + 1);
                readBuffer.erase(0, newlinePos + 1);
            }

            return lastMessage;
        }

        return ""; // no data available
    }

private:
    std::string devicePath;
    speed_t baudRate;
    int serialFd;
    std::string readBuffer;

    void writeString(const std::string& data) {
        ssize_t bytes_written = write(serialFd, data.c_str(), data.size());
        if (bytes_written != static_cast<ssize_t>(data.size())) {
            std::cerr << "Failed to write complete string ("<< bytes_written << "/" << data.size() << " bytes) to serial port\n";
        }
    }
};
