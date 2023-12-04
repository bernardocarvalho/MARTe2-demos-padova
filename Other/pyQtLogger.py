#!/usr/bin/env python3
"""
PyQt6 App for signing Esther Checklists
https://github.com/mcagriaksoy/PyQt--UDP-Receiver-Transmitter/blob/master/receiver.py
"""

import sys
import time

from PyQt6.QtNetwork import QUdpSocket, QHostAddress

from PyQt6.QtCore import QSize, Qt, QSortFilterProxyModel, QByteArray #View
from PyQt6.QtGui import QFont

from PyQt6.QtWidgets import (
    QWidget,
    QApplication,
    QLineEdit,
    QMainWindow,
    #QTableView,
    QVBoxLayout,
    QHBoxLayout,
    QDialog,
    QDialogButtonBox,
    QCheckBox,
    #QComboBox,
    QMessageBox,
    QPushButton,
    #QSpinBox,
    QRadioButton,
    QTabWidget,
    QLabel,
    QPlainTextEdit,
)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.logText = QPlainTextEdit(self)
        self.logText.setReadOnly(True)
        pushButtonConnect = QPushButton("Connect")
        #pushButtonConnect.clicked.connect(self.on_pushButton_clicked)
        #pushButtonStart = QPushButton("Start")
        #self.pushButton.clicked.connect(self.start_loop)
        pushButtonClear = QPushButton("Clear Log")
        pushButtonClear.clicked.connect(self.clear_log)
        QPushButton("Clear Log")
        #pushButtonStop.clicked.connect(self.stop_loop)  # stop the loop on the stop button click
        self.udpSocket = QUdpSocket(self)
        #udpSocket.bind(QHostAddress.LocalHost, 7755)
#        udpSocket.bind(45454, QUdpSocket.ShareAddress)
        self.udpSocket.bind(32767)
        self.udpSocket.readyRead.connect(
            self.readPendingDatagrams)

        #udpSocket.readyRead.connect(
        #    self.readPendingDatagrams)
        layoutTables = QVBoxLayout()
        label = QLabel('PyQt MARTe Logger')
        label.setFont(QFont('Arial', 20))

        layoutTables.addWidget(label)
        layoutTables.addWidget(self.logText)
        layoutTables.addWidget(pushButtonConnect)
        #layoutTables.addWidget(pushButtonStart)
        layoutTables.addWidget(pushButtonClear)
        layoutMain = QHBoxLayout()

        layoutMain.addLayout(layoutTables)

        container = QWidget()
        container.setLayout(layoutMain)
        self.setMinimumSize(QSize(1000, 400))
        self.setCentralWidget(container)

    def readPendingDatagrams(self):

        while self.udpSocket.hasPendingDatagrams():
            datagram = self.udpSocket.receiveDatagram()
            dataBa = datagram.data()
            dataLine = str(dataBa, encoding='utf-8')
            
            #print('Ix= ' + str(dataBa.indexOf(b"E=Warning")))
            #if(dataBa.contains(bvWarn)):
            if dataBa.indexOf(b"E=Error") != -1:
                #self.logText.setStyleSheet("QPlainTextEdit {background-color: black; color: red;}")
                dataHtml = f"<div style='color: red;'> {dataLine} </div>"
            elif dataBa.indexOf(b"E=FatalError") != -1:
                dataHtml = f"<div style='color: red;'> {dataLine} </div>"
            elif dataBa.indexOf(b"E=Warning") != -1:
                #self.logText.setStyleSheet("QPlainTextEdit {background-color: black; color: red;}")
                dataHtml = f"<div style='color: orange;'> {dataLine} </div>"
            else:
                dataHtml = f"<div style='color: green;'> {dataLine} </div>"

            self.logText.appendHtml(dataHtml)
            #self.logText.setStyleSheet("QPlainTextEdit {background-color: black; color: red;}")
            #self.logText.appendPlainText(dataLine)
            #processTheDatagram(datagram)
    def clear_log(self):
        self.logText.clear()


app = QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec()

# vim: syntax=python ts=4 sw=4 sts=4 sr et


