

#Poco libraries: /usr/lib
#Poco base:/usr/include/Poco; $POCO/Util
POCO_BASE_LIBS = -lPocoFoundation -lPocoUtil
#Poco Xml: depends on expat; $POCO/XML
POCO_XML_LIBS = -lexpat -lPocoXml
#Poco Crypto: depends on crypto: $POCO/Crypto
POCO_CRYPTO_LIBS = -lcrypto -lPocoCrypto
#Poco Net: depends on openssl: $POCO/Net
POCO_NET_LIBS = -lssl -lPocoNet -lPocoNetSSL
#Poco DATA ODBC: depends on unixODBC:$POCO/Data[/ODBC]
POCO_ODBC_LIBS = -lunixODBC -lPocoData -lPocoDataODBC
#Poco Data Sqlite3: depends sqlite3:$POCO/Data[/SQLite]
POCO_SQLITE_LIBS = -lsqlite3 -lPocoData -lPocoDataSQLite
#Poco Data MySQL : depends mysql:$POCO/Data[/MySQL]
POCO_MYSQL_LIBS = -lmysql -lPocoData -lPocoMySQL
#Poco Zip: depends on zip
POCO_ZIP_LIBS = -lzip -lPocoZip
#common flags:
#add include paths: -I${include_dirs}
#CPPFLAGS += 
#extra flags for c compile ${CC}
#CFLAGS +=
#extra flags for cxx compile ${CXX}
#CXXFLAGS += std=c++0x
#add library paths: -L${library_dirs}
#LDFLAGS +=
#add libraries: -l${library} 

#added Poco libraries:
#POCO_{BASE|ZIP|NET|CRYPTO|ODBC|SQLITE|XML}_LIBS
LDLIBS += 

