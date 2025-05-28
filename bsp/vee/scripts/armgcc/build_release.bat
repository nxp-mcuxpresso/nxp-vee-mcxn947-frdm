SET CMD=ninja

if not exist release mkdir release
cd release
cmake --preset release %_tail% ..
%CMD% 2> build_log.txt