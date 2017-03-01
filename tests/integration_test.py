import subprocess
import os
import telnetlib

subprocess.call('cd ..', shell=True)

print('Starting webserver...')
webserver = subprocess.Popen(['./webserver', 'config'], stdout=subprocess.DEVNULL)

print('TEST 1: Echoing request.')
print('Sending request...')
curl_proc = subprocess.Popen("curl -s localhost:8080", stdout=subprocess.PIPE, shell=True)

response = curl_proc.stdout.read().decode('utf-8')
expected = 'GET / HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\n';

print('Checking response...')
if response != expected:
	print('FAILED: Response does not match expected!')
	print('Expected: ' + expected)
	print('Response: ' + response)
	webserver.kill()
	exit(1)
else:
	print('SUCCESS!')

print('TEST 2: Serving static files.')
print('Sending request...')
curl_proc = subprocess.call(["curl", "-s", "localhost:8080/static/index.html", "-o", "output"])

diff_proc = subprocess.Popen(["diff", "-u", "file/path0/index.html", "output"], stdout=subprocess.PIPE)
diff = diff_proc.stdout.read().decode('utf-8')
os.remove('output')

print('Checking response...')
if diff != "":
	print('FAILED: Response does not match expected!')
	print(diff)
	webserver.kill()
	exit(1)
else:
	print('SUCCESS!')
	webserver.kill()
	exit(0)

print('TEST 3: Handling multiple requests at a time.')

first_half = 'GET /echo/ HTTP/1.1\r\nUser-Agent: telnet\r\nHost: localhost:8080\r\n'
second_half = 'Accept: */*\r\n\r\n'

print('Opening up a telnet connection...')
tn1 = telnetlib.Telnet('localhost', 8080, 2)
print('Sending a partial request...')
tn1.write(first_half.encode('ascii'))
response1 = tn1.read_eager() 

print('Opening up another telnet connection...')
tn2 = telnetlib.Telnet('localhost', 8080, 2)
print('Sending a complete request...')
tn2.write((first_half+second_half).encode('ascii'))
response2 = tn2.read_all().decode('utf-8')

print('Finishing the first request...')
tn1.write(second_half.encode('ascii'))
response1 = (response1+tn1.read_all()).decode('utf-8')

webserver.kill()

expected = 'HTTP/1.1 200 OK\r\nContent-Length: 78\r\nContent-Type: text/plain\r\n\r\nGET /echo/ HTTP/1.1\r\nUser-Agent: telnet\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\n'

print('Checking responses...')
if response1 != expected:
	print('FAILED: Response does not match expected!')
	print('Expected: ' + expected)
	print('Response: ' + response1)
	exit(1)
elif response2 != expected:
	print('FAILED: Response does not match expected!')
	print('Expected: ' + expected)
	print('Response: ' + response2)
	exit(1)
else:
	print('SUCCESS!')
	exit(0)
