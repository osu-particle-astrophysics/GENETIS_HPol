import sys
import time
import logging
from watchdog.observers import Observer
import os
from watchdog.events import LoggingEventHandler
from watchdog.events import (
    FileSystemEventHandler,
    FileCreatedEvent,
    FileDeletedEvent,
    FileMovedEvent,
    FileModifiedEvent,
)


class Watch(FileSystemEventHandler):
    def __init__(self, handler):
        print("init")
        self.handler = handler

    def on_created(self, event):
        super(Watch, self).on_created(event)
        if isinstance(event, FileCreatedEvent):
            self.handler(event)

    def on_deleted(self, event):
        super(Watch, self).on_deleted(event)
        if isinstance(event, FileDeletedEvent):
            self.handler(event)

    def on_moved(self, event):
        super(Watch, self).on_deleted(event)
        if isinstance(event, FileMovedEvent):
            self.handler(event)

    def on_modified(self, event):
        super(Watch, self).on_modified(event)
        if isinstance(event, FileModifiedEvent):
            self.handler(event)


def file_handler(event):
    # print("get event", event)
    if event.is_directory is False and event.src_path.endswith(".rst"):
        os.system("make html")


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s - %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )
    path = sys.argv[1] if len(sys.argv) > 1 else "."
    event_handler = LoggingEventHandler()
    observer = Observer()
    # observer.schedule(event_handler, path, recursive=True)
    filesystem_handler = Watch(file_handler)
    observer.schedule(filesystem_handler, path, recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(10000)
    finally:
        observer.stop()
        observer.join()
