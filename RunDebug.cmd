cmake --build . --config Debug
xcopy ".\Data" ".\Debug\Data" /E /I /Y
cls
.\Debug\StockMarketSimulator.exe
