cmake --build . --config Release
xcopy ".\Data" ".\Release\Data" /E /I /Y
cls
.\Release\StockMarketSimulator.exe

