#!/usr/bin/env python3
from IniBuffer import *

def main():
    ini = IniBuffer()
    ini.LoadFile("/home/pascal/Desktop/test.ini")
#------------------------------------------------------------------
# Request an int

    section_name = "04_integers"
    key_name = "key02"

    value = ini.GetValue_int(section_name, key_name)
    print("section:"+section_name+"; key:"+key_name+"; value:"+str(value))

#------------------------------------------------------------------
# Request a float

    section_name = "05_floats"
    key_name = "key02"

    value = ini.GetValue_float(section_name, key_name)
    print("section:"+section_name+"; key:"+key_name+"; value:"+str(value))

#------------------------------------------------------------------
# Request a bool

    section_name = "06_booleans"
    key_name = "key02"

    value = ini.GetValue_bool(section_name, key_name)
    print("section:"+section_name+"; key:"+key_name+"; value:"+str(value))

#------------------------------------------------------------------
# Request a string
    section_name = "03_strings"
    key_name = "key02"

    value = ini.GetValue_string(section_name, key_name)
    print("section:"+section_name+"; key:"+key_name+"; value:"+str(value))

#------------------------------------------------------------------
# Add a value
    section_name = "new_section"
    key_name = "key"

    ini.AddValue(section_name, key_name, 1.55)

    value = ini.GetValue_float(section_name, key_name)
    print("section:"+section_name+"; key:"+key_name+"; value:"+str(value))

if __name__ == "__main__":
	main()
