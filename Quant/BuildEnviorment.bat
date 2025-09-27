@echo off
set "VENV_NAME=venv"

echo Creating virtual environment '%VENV_NAME%'...
python -m venv %VENV_NAME%

echo Activating virtual environment...
call %VENV_NAME%\Scripts\activate.bat

echo Installing packages from requirements.txt...
pip install -r requirements.txt
