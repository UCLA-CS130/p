all: 
	g++ -std=c++11 -pthread webserver_main.cc webserver.cc -o webserver -lboost_system -lboost_regex

clean:
	rm -f *.o *.a webserver config_parser config_parser_test
