import subprocess
import time
import threading

def task_a(event):
    process = subprocess.Popen(['octave-cli', "mat_code.m"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # 等待Octave进程结束
    stdout, stderr = process.communicate()
    # 输出Octave的输出结果
    print(stdout.decode('utf-8'),end='')
    print(stderr.decode('utf-8'),end='')
    event.set()

def task_b(event):
    time.sleep(1)
    subprocess.run("kill -9 $(pgrep -f mat_code)",shell = True)
    print("Time Out")
    event.set()

# 创建事件对象
event = threading.Event()

# 创建线程
thread_a = threading.Thread(target=task_a, args=(event,))
thread_b = threading.Thread(target=task_b, args=(event,))

# 启动线程
thread_a.start()
thread_b.start()

# 等待任意一个线程完成
event.wait()

