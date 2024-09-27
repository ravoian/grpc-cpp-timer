call cmake -B build
call cmake --build build --config Release --parallel
start cmd /k build\Release\timer_server.exe
start cmd /k build\Release\timer_client.exe
@pause