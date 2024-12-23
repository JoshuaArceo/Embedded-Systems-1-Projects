import tkinter as tk
from tkinter import simpledialog
import threading
import socket
import time
import numpy as np
import matplotlib.pyplot as plt
import os
import re
import queue

def main():
    global window, plot_queue, message_log
    window = tk.Tk()
    window.title("Sensor Command and Control GUI")

    global last_command_label
    last_command_label = tk.Label(window, text="Last Command Sent: None")
    last_command_label.pack()

    message_log = tk.Text(window, height=10, width=50)
    message_log.pack()

    plot_queue = queue.Queue()
    add_movement_buttons()

    quit_command_button = tk.Button(window, text="Quit", command=send_quit)
    quit_command_button.pack()

    threading.Thread(target=socket_thread, daemon=True).start()
    window.after(100, process_queue)
    window.mainloop()

def add_movement_buttons():
    buttons = {
        'Forward (w)': 'w',
        'Backward (s)': 's',
        'Turn Left (a)': 'a',
        'Turn Right (d)': 'd',
        'Start Scan (g)': 'g',
        'End (m)' : 'm',
        'Read (r)': 'r'
    }
    for label, command in buttons.items():
        btn = tk.Button(window, text=label, command=lambda cmd=command: send_command(cmd))
        btn.pack()

def send_command(cmd):
    global gui_send_message
    gui_send_message = cmd + '\n'
    last_command_label.config(text=f"Last Command Sent: {cmd}")

def send_quit():
    global gui_send_message
    global window
    gui_send_message = 'quit\n'
    time.sleep(1)
    window.destroy()

def socket_thread():
    global gui_send_message
    global last_command_label

    HOST = '192.168.1.1'
    PORT = 288
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        cybot = s.makefile('rwb', buffering=0)
        gui_send_message = 'wait\n'

        while True:
            if gui_send_message != 'wait\n':
                if gui_send_message != 'r':
                    cybot.write(gui_send_message.encode())
                if gui_send_message.strip() == 'quit':
                    break
                if gui_send_message.strip() == 'g':
                    handle_scan(cybot)
                else:
                    response = cybot.readline().decode().strip()
                    if response:
                        update_gui_log(response)
                    else:
                        response = cybot.readline().decode().strip()
                        update_gui_log(response)
                gui_send_message = 'wait\n'
            time.sleep(0.1)
        cybot.close()

def update_gui_log(message):
    # Insert the message into the text widget
    message_log.insert(tk.END, message + '\n')
    message_log.see(tk.END)  # Scroll to the bottom

def readline(cybot):
    line = cybot.readline().decode().strip()
    print(line)

def handle_scan(cybot):
    header = cybot.readline().decode().strip()  # Read header and ignore it
    data = ""
    while True:
        line = cybot.readline().decode().strip()
        if line == "END":
            update_gui_log("Scan Finished")
            break
        data += line + '\n'
    plot_queue.put(lambda: update_plots(data, header))


def handle_movement(cybot):
    response = cybot.readline().decode().strip()
    print(response)  # This ensures that even movement responses are printed immediately


def update_plots(data, header):
    angles, distances, ir_distances = [], [], []
    for line in data.strip().splitlines():
        angle, dist, _, ir_dist = line.split()
        print(dist)
        print(ir_dist)

        angle = float(angle)
        dist = float(re.sub(r'[a-zA-Z]', '', dist))  # Strip non-numeric chars
        ir_dist = float(re.sub(r'[a-zA-Z]', '', ir_dist))  # Strip non-numeric chars
        distances.append(dist / 100)  # Convert cm to meters
        if abs(ir_dist - dist) < 15:
            ir_distances.append(ir_dist / 100)  # Convert cm to meters
        else:
            ir_distances.append(99999)  # Convert cm to meters
        angles.append(angle)
    angles = np.deg2rad(angles)

    fig, ax1 = plt.subplots(subplot_kw={'projection': 'polar'})
    ax1.plot(angles, distances, 'r.-')
    ax1.set_rmax(1.5)
    ax1.set_rticks([0.25, 0.5, 0.75, 1.0, 1.25, 1.5])
    ax1.set_title(f"Ultrasonic Sensor Data: {header}")
    ax1.set_thetamax(180)

    fig, ax2 = plt.subplots(subplot_kw={'projection': 'polar'})
    ax2.plot(angles, ir_distances, 'b.-')
    ax2.set_rmax(1.5)
    ax2.set_rticks([0.25, 0.5, 0.75, 1.0, 1.25, 1.5])
    ax2.set_title(f"IR Sensor Data: {header}")
    ax2.set_thetamax(180)

    plt.show(block=False)

def process_queue():
    try:
        while True:
            task = plot_queue.get_nowait()
            task()
    except queue.Empty:
        pass
    finally:
        window.after(100, process_queue)

if __name__ == '__main__':
    main()
