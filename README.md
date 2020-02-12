

# IniBuffer [![Build Status](https://travis-ci.org/pascalenderli/IniBuffer.svg?branch=master)](https://travis-ci.org/pascalenderli/IniBuffer)
This is a C++ class, which exposes a public interface to read an ini configuration file into a buffer, access or manipulate its content or writing a buffer to a formatted ini-file on the hard-disk.

## Background
Ini files are are simple, human readable configuration files. They are widely used in computer applications to store system configurations on the hard disk.

## Format
The ini Format is not well defined. Different systems interpret the files slightly different or support different features.
This file parser is optimized for simple usage and does only support a minimal but suffisent set of ini features.

In general an inifile consists of Sections and Properties.

### Sections
Sections are parenthesized with square brakets to the left and right of the unique name string. Each section can be empty or  contain a set of properties. Section names can contain white spaces in between words. White spaces in front of the first letter or after the last letter are ignored.

#### Valid Sections:

```text
[Section]
[  Section  ] -> Aequivalent to:  [Section]
[A_Section]
[A Section]
[]
["  "]
[1]
```
#### Invalid Sections:
```text
Section
[Section
Section ]
]Section[
```

Never the less using white spaces in name strings is considered bad practise.

### Properties
Properties are Key|Value - pairs which belong to a section. This Parser does only support ini-files which contain at least one section. therefore properties must belong to a section.

The parser ignoreas any amount of white spaces in front or after keys or values. Whitespaces in between key names are not valid. Whitspaces in values are valid and belong to the value string.

#### Valid  Properties:
```text
key1=value
an_int = 7
name = Alice
 another_name   = Bob
pi = 3.1415
switch = true 
```
#### Invalid  Properties:
```text
price of a phone = 1000
```
### Comments
Comments start with a '#' or a ';'

### Valid ini-File Example
shapes.ini
```text
# This is a dummy ini file.
# Its not pretty but it shows some ini specific stuff.
; this is also a comment.

[Circle]           # A Shape.
Diameter = 700     ; A comment.
Color = red
has_edges = false

[ Square ]         # Another comment.
color= "green"
material = wood 
has_edges=true
```

## Interface Usage
There are only a few interface functions described below. If a key or section is duplicaed, the values are overwritten. A key feature of this IniBuffer is the automatic type conversion of parsed values.

### Constructor
Constructs an empty IniBuffer object.
```cpp
IniBuffer();
```

### Loading a file from disk
Parses the ini-file into the buffer.
Integer, floats, and bools (true, false, TRUE, FALSE, True, False) are automatically typecasted to its ptoper types. all ather values are treated as strings.
```cpp
void LoadFile(const std::string& fullfilename);
```

### Writing a file to disk
Writes the whole content of the buffer to a .ini file on the disk.
```cpp
void WriteFile(const std::string& fullfilename) const;
```

### Clearing the buffer
Removes all data in the buffer.
```cpp
void Clear() noexcept;
```

### Returning a value from the buffer
A value can be requested from the ini file to store it in another variable. In order to do this, The function GetValue must be called. It accepts two arguments used to ocate the value. The section name and the key name.
This function casts the value automatically to the type of the variable. If the requested type does not match the type of the value, an exception is thrown. The supported datatypes are bool, int, float ond std::sting.
```cpp
template<typename ValueT>
ValueT GetValue(const std::string& section_name, const std::string& key_name) const;
```

### Adding or Updating a value in the buffer
Ads a new value to the buffer. Any of the supported data types can be passed. The supported datatypes are bool, int, float and std::sting.
```cpp
template<typename ValueT>
void AddValue(const std::string& section_name, const std::string& key_name, const ValueT& value);
```

## Exceptions
The Class returns an Exception of type IniException if something goes wrong.
The what() methods returns a meaningful Error message of the form: `[IniBufferException][File: <filename>][Line: <line_nr>][What: <message>]`

```cpp
try
{
    IniBuffer ini;
    ini.LoadFile("../data/test.ini");
    ini.AddValue("added_test_section", "key_100", std::string("a text"));
    ini.WriteFile("../results/test_result.ini");
}
catch(IniException& e)
{
    std::cout<<e.what()<<'\n';
}
```

## Example

```cpp
try
{
    IniBuffer ini;
    ini.LoadFile("shapes.ini");
    ini.AddValue("Circle", "material", std::string("stone"));
    ini.WriteFile("shapes_copy.ini");

    int d = ini.GetValue<int>("Cercle", "Diameter");
    std::string c = ini.GetValue<std::string>("Cercle", "Solor");
    std::cout<<"Diameter: "<<d<<"\n"<<"color: "<<c<<"\n";
}
catch(IniException& e)
{
    std::cout<<e.what()<<'\n';
    abort();
}
```
```text
$ g++ main.cpp -O3
$ ./a.out
Diameter: 700
Color: red
```
