SET CMD=ninja

if not exist debug mkdir debug
cd debug
cmake --preset debug %_tail% ..
%CMD%