# define target, etc vars
include makefile.vars

##  std flags
include makefile.std

##############################
#added Poco libraries:
#POCO_{BASE|ZIP|NET|CRYPTO|ODBC|SQLITE|XML}_LIBS
include makefile.poco

###############################################
main:app
	
include makefile.targets


