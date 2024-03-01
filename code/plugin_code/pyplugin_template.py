
import sys
import io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf8')
del sys


def plugin_main(str) -> str:
    pass

if __name__ == "__main__":
    fp = open("Current_msg.txt", "r", encoding='utf-8')
    p = fp.read()
    print(plugin_main(p))
