import subprocess
import os
import telnetlib
import sys

subprocess.call('cd ..', shell=True)

print('Starting webserver...')
webserver = subprocess.Popen(['./webserver', 'config'], stdout=subprocess.DEVNULL)

print('TEST: Handling multiple requests at a time.')

first_half = 'GET /echo/ HTTP/1.1\r\nUser-Agent: telnet\r\nHost: localhost:8080\r\n'
second_half = 'Accept: */*\r\n\r\n'
expected = 'HTTP/1.1 200 OK\r\nContent-Length: 78\r\nContent-Type: text/plain\r\n\r\nGET /echo/ HTTP/1.1\r\nUser-Agent: telnet\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\n'

print('Opening up a telnet connection...')
tn1 = telnetlib.Telnet('localhost', 8080, 5)
print('Sending a partial request...')
tn1.write(first_half.encode('ascii'))
response1 = tn1.read_eager() 

for x in range(int(sys.argv[1])-1):
	print('Opening up paralell telnet connection {}...'.format(x+2))
	tn2 = telnetlib.Telnet('localhost', 8080, 2)
	print('Sending a complete request...')
	tn2.write((first_half+second_half).encode('ascii'))
	response2 = tn2.read_all().decode('utf-8')
	print('Checking response...')
	if response2 != expected:
		print('FAILED: Response does not match expected!')
		print('Expected: ' + expected)
		print('Response: ' + response2)
		webserver.kill()
		exit(1)
	else:
		print('Paralell request {} succeeded...'.format(x+2))

print('Finishing the first request...')
tn1.write(second_half.encode('ascii'))
response1 = (response1+tn1.read_all()).decode('utf-8')

webserver.kill()

print('Checking responses...')
if response1 != expected:
	print('FAILED: Response does not match expected!')
	print('Expected: ' + expected)
	print('Response: ' + response1)
	exit(1)
else:
	print('SUCCESS!')
	exit(0)