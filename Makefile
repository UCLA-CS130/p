all: 
	g++ -std=c++11 -pthread webserver_main.cc webserver.cc -o webserver -lboost_system

clean:
	rm -f *.o *.a webserver config_parser config_parser_test
