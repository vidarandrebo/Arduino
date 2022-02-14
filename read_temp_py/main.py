import serial

import sys
import time
import traceback
from os import environ

from PyQt5.QtCore import QRunnable, QObject, pyqtSignal, pyqtSlot, QThreadPool
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QWidget, QProgressBar


def suppress_qt_warnings() -> None:
    environ["QT_DEVICE_PIXEL_RATIO"] = "0"
    environ["QT_AUTO_SCREEN_SCALE_FACTOR"] = "1"
    environ["QT_SCREEN_SCALE_FACTORS"] = "1"
    environ["QT_SCALE_FACTOR"] = "1"


class WorkerSignals(QObject):
    finished = pyqtSignal()
    error = pyqtSignal(tuple)
    result = pyqtSignal(object)
    progress = pyqtSignal(float)


class Worker(QRunnable):
    def __init__(self, fn, *args, **kwargs):
        super(Worker, self).__init__()
        self.fn = fn
        self.args = args
        self.kwargs = kwargs
        self.signals = WorkerSignals()
        self.kwargs['progress_callback'] = self.signals.progress

    @pyqtSlot()
    def run(self) -> None:
        # runtime exceptions are for when the gui is removed from the main function
        try:
            result = self.fn(*self.args, **self.kwargs)
        except:
            try:
                traceback.print_exc()
                exc_type, value = sys.exc_info()[:2]
                self.signals.error.emit((exc_type, value, traceback.format_exc()))
            except RuntimeError:
                pass
        else:
            try:
                self.signals.result.emit(result)
            except RuntimeError:
                pass
        finally:
            try:
                self.signals.finished.emit()
            except RuntimeError:
                pass


class MainWindow(QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.running = True
        self.init_gui()
        self.drill_bit_rpm = QLabel()
        self.progress = QProgressBar()
        self.create_gui_elements()
        self.show()
        self.threadpool = QThreadPool()
        self.add_initial_threads()

    def add_initial_threads(self) -> None:
        worker = Worker(self.main_loop)
        worker.signals.progress.connect(self.set_temp_value)
        self.threadpool.start(worker)

    def init_gui(self) -> None:
        self.setWindowTitle("Drillbotics Control System")
        self.setGeometry(300, 300, 300, 100)

    def create_gui_elements(self) -> None:
        layout = QVBoxLayout()
        self.drill_bit_rpm.setText(f"{1}")
        self.progress.setMaximum(100)
        self.progress.setToolTip("Progress")
        layout.addWidget(self.progress)
        layout.addWidget(self.drill_bit_rpm)
        window = QWidget()
        window.setLayout(layout)
        self.setCentralWidget(window)

    def set_temp_value(self, n: float) -> None:
        self.drill_bit_rpm.setText(f"{n}")
        self.progress.setValue(int(n))

    def main_loop(self, progress_callback: pyqtSignal) -> str:
        a = 0.0
        serial_conn = serial.Serial('COM5', 9600, timeout=1)
        while self.running:
            line = serial_conn.readline().decode().strip()
            if line:
                a = float(line)
                print(a)
                try:
                    progress_callback.emit(a)
                except AttributeError:
                    break
            time.sleep(0.25)

        serial_conn.close()
        return "Done"


def main() -> None:
    suppress_qt_warnings()
    app = QApplication([])
    window = MainWindow()
    app.exec_()
    window.running = False


if __name__ == '__main__':
    main()
