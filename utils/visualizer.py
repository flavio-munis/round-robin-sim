import json
import sys

def main(path):
    fp = open(path, "r")
    data = json.load(fp)

    print(path)
    fp.close()

if __name__ == "__main__":
    for arg in sys.argv:
        main(arg)
