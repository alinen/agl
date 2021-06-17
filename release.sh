#! bash
doxygen Doxyfile
python3 cpplint.py agl/*.h
python3 cpplint.py agl/*.cpp
python3 cpplint.py agl/mesh/*.h
python3 cpplint.py agl/mesh/*.cpp
python3 cpplint.py examples/*.cpp
