@echo off
echo Compiling...
g++ -o x.exe mainUI.cpp classes1.cpp Database.cpp MenuItemDAO.cpp RestaurantDAO.cpp sqlite3.o User.cpp UserDAO.cpp
if %errorlevel% neq 0 (
    echo Error In Compile!
    pause
    exit /b
)
echo Running...
x.exe
pause