SET CMD=ninja

set _all=%*
call set _tail=%%_all:%1 =%%

if not exist debug mkdir debug
cd debug
cmake --preset debug %_tail%  ..
%CMD%
cd ..\

if not exist debug mkdir debug
cd debug
cmake --preset release %_tail%  ..
%CMD%

IF "%1" == "" ( pause )
