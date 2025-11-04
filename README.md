# File Indexer

A small practice project I want to use to familiarize myself with boost.
It is aimed to be a command line tool that indexes files in a directory and allows searching and filtering based on various criteria.

---

## (Planned) Features

- Recursively index directories
- Search files by name (regex support)
- Filter by size or modification date
- Sort results by name, date, or size
- Configurable via command-line options

---

## Built With

- **C++20**

---

## Build Instructions

```bash
git clone https://github.com/Solek798/File-Indexer.git
cd File-Indexer
mkdir build && cd build
cmake ..
make
./file_indexer --path ~/Documents --name ".*\\.cpp" --min-size 1024
