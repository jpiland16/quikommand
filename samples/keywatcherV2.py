from pynput.keyboard import Key, Listener
import time
import os
import win32gui, win32con
import msvcrt
import sys
from infi.systray import SysTrayIcon
from threading import Thread

userQuit = False

def on_quit_callback(systray):
    global userQuit
    userQuit = True

systray = SysTrayIcon("quikommand-01.ico", "keywatcher is running", None, on_quit=on_quit_callback)
systray.start()

console_window = win32gui.GetForegroundWindow()
print("Starting keywatcher...\n")
time.sleep(2)

win32gui.ShowWindow(console_window , win32con.SW_HIDE)

last_esc_press = 0
last_ctrl_press = 0

prompted_before = False

MAX_INTERVAL = 0.200 # seconds

# From https://stackoverflow.com/questions/15528939/python-3-timed-input

class TimeoutExpired(Exception):
    pass

def input_with_timeout(prompt, timeout, timer=time.monotonic):
    sys.stdout.write(prompt)
    sys.stdout.flush()
    endtime = timer() + timeout
    result = []
    while timer() < endtime:
        if msvcrt.kbhit():
            result.append(msvcrt.getwche()) #XXX can it block on multibyte characters?
            if result[-1] == '\r':
                return ''.join(result[:-1])
        time.sleep(0.04) # just to yield to other processes/threads
    raise TimeoutExpired

# ---------------------------------------------------------------------

def on_press(key):
    global last_ctrl_press, last_esc_press, prompted_before
    if key == Key.ctrl_l:
        now = time.time()
        if now - last_ctrl_press < MAX_INTERVAL:
            os.system(r'start cmd /K "cd "<PATH_TO_QUIKOMMAND_FOLDER>" & quikommand.exe & exit"')
        else:
            last_ctrl_press = now
    elif key == Key.esc:
        now = time.time()
        if now - last_esc_press < MAX_INTERVAL:
            win32gui.ShowWindow(console_window , win32con.SW_SHOW) 
            try: 
                prompt = "Press ENTER to exit. The program will automatically resume in 5 seconds." if not prompted_before else ""
                prompted_before = True
                input_with_timeout(prompt, 5)
                return False
            except TimeoutExpired:
                win32gui.ShowWindow(console_window , win32con.SW_HIDE)
                last_esc_press = now
        else:
            last_esc_press = now

with Listener(on_press=on_press) as listener:
    def systray_quit_helper():
        while not userQuit:
            time.sleep(10)
        listener.stop()

    Thread(target=systray_quit_helper).start()
    listener.join()

systray.shutdown()