@echo off
chcp 65001
cd /d "%~dp0"
gcc main.c src/*.c windows/*.c -I include -o resturent.exe
resturent.exe