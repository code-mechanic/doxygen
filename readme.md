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
HIDE_SCOPE_NAMES       = YES
SORT_BRIEF_DOCS        = YES
INPUT                  = ../src
GENERATE_LATEX         = NO
```

## Running doxygen
- `doxygen`