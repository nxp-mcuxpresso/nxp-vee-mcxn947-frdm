# Properties

###############################################################################
# 						Testsuite Configuration Options
#
# Usage: 
#   Uncomment and set options marked '[required]'. 
#   Other options are set with a value that shall not be changed by default.
# 
# Each option can be declared outside this file as a system property (in the 
# gradle.properties with the `systemProp.` prefix for example).
# This can be useful to avoid to share absolute file-system locations in this file.
# In this case, it overrides the option defined in this file if any.
# Moreover, options related to local setup (absolute file-system locations, trace ip & port)
# can be extracted to the file 'local.properties' which is ignored for source control.   
#
# This file respects the Java properties file convention: the OS path
# must use the UNIX path convention (path separator is '/'). The WINDOWS
# paths must have been converted manually replacing '\' by '/' or by '\\'.
###############################################################################

###############################################################################
# BSP Connection [required]
# Uncomment one and only one option block depending on how the target VEE Port is connected to BSP.  
# See https://docs.microej.com/en/latest/VEEPortingGuide/platformCreation.html
###############################################################################

# No BSP Connection
#microej.testsuite.properties.deploy.dir.microejapp=[absolute_path]
#microej.testsuite.properties.deploy.dir.microejlib=[absolute_path]
#microej.testsuite.properties.deploy.dir.microejinc=[absolute_path]
#microej.testsuite.properties.deploy.dir.microejscript=[absolute_path]

# Partial BSP Connection
#microej.testsuite.properties.deploy.bsp.root.dir=[absolute_path]
#microej.testsuite.properties.deploy.bsp.microejscript=true

# Full BSP Connection
#microej.testsuite.properties.deploy.bsp.microejscript=true

###############################################################################
# Trace Redirection (System.out)
###############################################################################

# Uncomment and define these 2 properties when trace is redirected by 'Serial to Socket Transmitter' tool
#microej.testsuite.properties.testsuite.trace.ip=localhost
#microej.testsuite.properties.testsuite.trace.port=5555
# Otherwise uncomment this property if the trace is redirected by 'run.bat' or 'run.sh' script.
#microej.testsuite.properties.launch.test.trace.file=true

# VEE Port specific option to redirect trace on dedicated UART
#microej.testsuite.properties.debug.traces.uart=SET

#######################################################################
# Advanced Options
# These options shall not be changed by default.
###############################################################################

# The deploy tool to run after the build of the microejapp.o (defaults to 'Platform Configuration Additions')
microej.testsuite.properties.microejtool.deploy.name=deployToolBSPRun

# Set the verbose or not. Possible values: `true` or `false`
# When this option is set to `true`, the harness will output the execution trace.
microej.testsuite.verbose=true

# The testsuite timeout (in seconds)
microej.testsuite.timeout=600

# The testsuite trace timeout (in seconds)
microej.testsuite.properties.testsuite.trace.timeout=75

# The number of times we'll retry a test if it fails
microej.testsuite.retry.count=1

# Retry a test unless this pattern is shown
microej.testsuite.retry.unless=MicroEJ START

# A jvm args to pass to the testsuite harness
microej.testsuite.jvmArgs=-Xmx768m

# A jvm args to pass to launch scripts forked vm
microej.testsuite.properties.launch.properties.jvm=-Xmx2048M
