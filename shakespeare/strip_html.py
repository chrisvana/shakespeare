# Copyright 2013
# Author: Christopher Van Arsdale

from HTMLParser import HTMLParser
import sys

class MLStripper(HTMLParser):
    def __init__(self):
        self.reset()
        self.fed = []
    def handle_data(self, d):
        self.fed.append(d)
    def get_data(self):
        return ''.join(self.fed)

def strip_tags(html):
    s = MLStripper()
    s.feed(html)
    return s.get_data()

def main():
    full_text = ""
    for line in sys.stdin:
        full_text += line
    print strip_tags(full_text)

if __name__ == "__main__":
    main()
