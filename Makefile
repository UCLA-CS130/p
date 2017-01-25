CC = g++
CFLAGS = -std=c++11

all: webserver build_configparser build_configparser_test

clean:
	rm -f *.o *.a webserver config_parser config_parser_test

webserver: 
	$(CC) $(CFLAGS) -pthread webserver_main.cc webserver.cc config_parser.cc -o webserver -lboost_system -lboost_regex

build_configparser:
	./build.sh

build_configparser_test:
	./build_tests.sh
