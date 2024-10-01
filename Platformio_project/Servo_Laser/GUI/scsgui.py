import tkinter as tk
from tkinter import messagebox
import serial
import serial.tools.list_ports
import threading
import sys

# Изначально серийное соединение не установлено
ser = None

# Определяем начальное значение порта в зависимости от ОС
if sys.platform.startswith('win'):
    default_port = "COM3"  # Для Windows
else:
    default_port = "/dev/ttyACM0"  # Для Linux и macOS

# Словарь соответствия скоростей
speed_dict = {
    "1 (25 мм/с)": 100,
    "2 (50 мм/с)": 200,
    "3 (75 мм/с)": 300,
    "4 (100 мм/с)": 400,
    "5 (125 мм/с)": 500,
    "6 (150 мм/с)": 600,
    "7 (175 мм/с)": 700,
    "8 (200 мм/с)": 800,
    "9 (225 мм/с)": 900,
    "10 (250 мм/с)": 1000
}

# Цветовые настройки
background_color = "#2C3E50"  # Темно-синий
frame_color = "#34495E"       # Синий-серый
button_color = "#1ABC9C"      # Бирюзовый
button_text_color = "#FFFFFF"
label_text_color = "#ECF0F1"  # Светлый серо-голубой
entry_bg_color = "#ECF0F1"
entry_fg_color = "#2C3E50"
optionmenu_bg_color = "#ECF0F1"
optionmenu_fg_color = "#2C3E50"

# Стиль для улучшенного интерфейса
btn_style = {
    "font": ("Arial", 12, "bold"),
    "bg": button_color,
    "fg": button_text_color,
    "activebackground": "#16A085",
    "activeforeground": button_text_color,
    "bd": 0,
    "relief": "flat",
    "cursor": "hand2"
}
entry_style = {
    "font": ("Arial", 12),
    "bg": entry_bg_color,
    "fg": entry_fg_color,
    "bd": 0,
    "relief": "flat",
    "highlightthickness": 1,
    "highlightbackground": "#95A5A6",
    "highlightcolor": "#1ABC9C"
}
label_style = {
    "font": ("Arial", 12),
    "bg": frame_color,
    "fg": label_text_color
}

def check_connection():
    """Проверяет соединение и обновляет цвет кнопки."""
    global ser
    if ser and ser.is_open:
        connect_button.config(bg="#28A745", activebackground="#218838", text="Соединение установлено")
    else:
        connect_button.config(bg="#DC3545", activebackground="#C82333", text="Соединение отсутствует")
    root.after(1000, check_connection)  # Проверка каждые 1 секунду

def connect_serial():
    """Попытка соединения с серийным портом."""
    global ser
    port = port_entry.get()
    try:
        if ser and ser.is_open:
            ser.close()  # Закрыть текущее соединение, если оно открыто
        ser = serial.Serial(port, 115200)  # Попытка открыть новое соединение
        connect_button.config(bg="#28A745", activebackground="#218838", text="Соединение установлено")
    except serial.SerialException:
        ser = None
        messagebox.showerror("Ошибка", f"Не удалось подключиться к {port}.")
        connect_button.config(bg="#DC3545", activebackground="#C82333", text="Соединение отсутствует")

def send_command(command):
    """Отправляет команду на устройство."""
    try:
        global ser
        if ser and ser.is_open:
            ser.write(command.encode())
            ser.flush()
        else:
            messagebox.showwarning("Внимание", "Соединение с устройством не установлено!")
    except serial.SerialException as e:
        messagebox.showerror("Ошибка передачи", f"Не удалось отправить команду: {e}")
        ser = None
    except OSError as e:
        messagebox.showerror("Ошибка системы", f"Ошибка ввода/вывода: {e}")
        ser = None

def rotate_clockwise():
    speed = speed_dict[speed_var.get()]  # Получаем скорость из выбранного значения
    send_command(f'F{speed}\n')  # Команда на вращение по часовой стрелке

def rotate_counterclockwise():
    speed = speed_dict[speed_var.get()]  # Получаем скорость из выбранного значения
    send_command(f'B{speed}\n')  # Команда на вращение против часовой стрелки

def stop_rotation():
    send_command('S\n')  # Команда на остановку

# Создание главного окна
root = tk.Tk()
root.title("Управление сервоприводом")
root.geometry("500x500")
root.resizable(True, True)
root.configure(bg=background_color)

# Заголовок приложения
title_label = tk.Label(root, text="Управление сервоприводом", font=("Arial", 18, "bold"), bg=background_color, fg=label_text_color)
title_label.pack(pady=(20, 10))

# Создание фрейма для соединения
connection_frame = tk.Frame(root, bg=frame_color)
connection_frame.pack(padx=20, pady=10, fill="x")

# Поле ввода для выбора серийного порта
port_label = tk.Label(connection_frame, text="Серийный порт:", **label_style)
port_label.grid(row=0, column=0, sticky="e", pady=5)
port_entry = tk.Entry(connection_frame, **entry_style)
port_entry.grid(row=0, column=1, pady=5, padx=10)
port_entry.insert(0, default_port)

# Кнопка соединения
connect_button = tk.Button(connection_frame, text="Соединить", command=connect_serial, font=("Arial", 12, "bold"), bg="#DC3545", fg=button_text_color, activebackground="#C82333", activeforeground=button_text_color, bd=0, width=20, relief="flat", cursor="hand2")
connect_button.grid(row=1, column=0, columnspan=2, pady=15)

# Центрирование элементов в connection_frame
connection_frame.grid_columnconfigure(0, weight=1)
connection_frame.grid_columnconfigure(1, weight=1)

# Создание фрейма для управления сервоприводом
control_frame = tk.Frame(root, bg=frame_color)
control_frame.pack(padx=20, pady=10, fill="x")

# Выбор скорости через OptionMenu
speed_label = tk.Label(control_frame, text="Выберите скорость:", **label_style)
speed_label.grid(row=0, column=0, sticky="e", pady=5)
speed_var = tk.StringVar(root)
speed_var.set("1 (25 мм/с)")  # Начальное значение

# Изменение визуала OptionMenu
speed_menu = tk.OptionMenu(control_frame, speed_var, *speed_dict.keys())
speed_menu.config(font=("Arial", 12), bg=optionmenu_bg_color, fg=optionmenu_fg_color, activebackground="#BDC3C7", activeforeground=optionmenu_fg_color, bd=0, highlightthickness=1, highlightbackground="#95A5A6")
speed_menu["menu"].config(bg=optionmenu_bg_color, fg=optionmenu_fg_color, activebackground="#BDC3C7", activeforeground=optionmenu_fg_color)
speed_menu.grid(row=0, column=1, pady=5, padx=10, sticky="ew")

# Центрирование элементов в control_frame
control_frame.grid_columnconfigure(0, weight=1)
control_frame.grid_columnconfigure(1, weight=1)

# Кнопки управления
btn_clockwise = tk.Button(control_frame, text="Вращать по часовой", command=rotate_clockwise, **btn_style, width=20)
btn_clockwise.grid(row=1, column=0, columnspan=2, pady=5)

btn_counterclockwise = tk.Button(control_frame, text="Вращать против часовой", command=rotate_counterclockwise, **btn_style, width=20)
btn_counterclockwise.grid(row=2, column=0, columnspan=2, pady=5)

btn_stop = tk.Button(control_frame, text="Остановить", command=stop_rotation, font=("Arial", 12, "bold"), bg="#E74C3C", fg=button_text_color, activebackground="#C0392B", activeforeground=button_text_color, bd=0, width=20, relief="flat", cursor="hand2")
btn_stop.grid(row=3, column=0, columnspan=2, pady=10)

# Постоянная проверка состояния соединения
check_connection()

# Запуск основного цикла приложения
root.mainloop()
