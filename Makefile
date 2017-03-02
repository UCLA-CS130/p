CC = g++
FLAGS = -std=c++11 -Wall -Werror -g -pthread
GTEST_DIR=googletest/googletest
TEST_DIR=tests

run: webserver

all: webserver configparser configparser_test webserver_test

config_parser: config_parser.o config_parser_main.o 
	$(CC) $(FLAGS) $^ -o $@

config_parser_test: config_parser.o 
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/config_parser_test.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

webserver: config_parser.o webserver.o webserver_main.o request.o response.o request_handler.o echo_handler.o static_handler.o not_found_handler.o status_handler.o proxy_handler.o http_client.o log.o
	$(CC) $(FLAGS) $^ -o webserver -lboost_system -lboost_regex

webserver_test: webserver.o
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/webserver_test.cc webserver.cc config_parser.cc request.cc response.cc echo_handler.cc static_handler.cc not_found_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage

%.o: %.cc
	$(CC) $(FLAGS) -c $<

integration_test: webserver
	python3 ${TEST_DIR}/integration_test.py

unit_test_coverage:
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/request_test.cc request.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./request_test 
	gcov -r request.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o response_test -fprofile-arcs -ftest-coverage
	./response_test 
	gcov -r response.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/echo_handler_test.cc request.cc response.cc request_handler.cc echo_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_handler_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./echo_handler_test 
	gcov -r echo_handler.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/static_handler_test.cc not_found_handler.cc request.cc response.cc request_handler.cc static_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o static_handler_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./static_handler_test 
	gcov -r static_handler.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/status_handler_test.cc request_handler.cc status_handler.cc request.cc response.cc log.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o status_handler_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./status_handler_test 
	gcov -r status_handler.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/log_test.cc log.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o log_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./log_test 
	gcov -r log.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/not_found_handler_test.cc not_found_handler.cc request_handler.cc request.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o not_found_handler_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./not_found_handler_test 
	gcov -r not_found_handler.cc
	$(CC) $(FLAGS) -isystem ${GTEST_DIR}/include -pthread $^ ${TEST_DIR}/webserver_test.cc webserver.cc config_parser.cc request.cc response.cc request_handler.cc echo_handler.cc static_handler.cc not_found_handler.cc status_handler.cc log.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -lboost_regex  -fprofile-arcs -ftest-coverage
	./webserver_test
	gcov -r webserver.cc

clean:
	rm -rf *.dSYM *.o *.a *.gcno *.gcov *.gcda config_parser webserver *_test

.PHONY: clean run all integration_test unit_test_coverage
