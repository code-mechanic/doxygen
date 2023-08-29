## Doxygen installation  
- For Ubuntu  
    - `sudo apt-get install doxygen`
- For CentOS  
    - `sudo yum install doxygen`

## Create a default Doxygen configuration file
- `doxygen -g`

## Basic setting for python documentation
```
PROJECT_NAME           = "My Project"
JAVADOC_AUTOBRIEF      = YES
OPTIMIZE_OUTPUT_JAVA   = YES
EXTRACT_ALL            = YES
EXTRACT_STATIC         = YES
HIDE_SCOPE_NAMES       = YES
SORT_BRIEF_DOCS        = YES
INPUT                  = ../src
RECURSIVE              = YES
GENERATE_LATEX         = NO
```
## Graphical stuffs
- To enable class diagram and call graph install graphviz  
    `sudo apt-get install graphviz`
- After graphviz installed make sure following settings are enabled in Doxyfile
```
CLASS_DIAGRAMS         = YES
HAVE_DOT               = YES
CLASS_GRAPH            = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
```
## Running doxygen
- `doxygen Doxyfile`