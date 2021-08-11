#! bash
doxygen Doxyfile
python3 cpplint.py --quiet agl/*.h
python3 cpplint.py --quiet agl/*.cpp
python3 cpplint.py --quiet agl/mesh/*.h
python3 cpplint.py --quiet agl/mesh/*.cpp
python3 cpplint.py --quiet examples/*.cpp
