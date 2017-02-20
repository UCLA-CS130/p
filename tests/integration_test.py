import subprocess
import os

subprocess.call('cd ..', shell=True)

print('Starting webserver...')
webserver = subprocess.Popen(['./webserver', 'config'], stdout=subprocess.DEVNULL)

print('TEST 1: Echoing request')
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

print('TEST 2: Serving static files')
print('Sending request...')
curl_proc = subprocess.Popen(["curl", "-s", "localhost:8080/static/index.html", "-o", "output"])

diff_proc = subprocess.Popen(["diff", "-u", "file/path0/index.html", "output"], stdout=subprocess.PIPE)
diff = diff_proc.stdout.read().decode('utf-8')

print('Checking response...')
if diff != "":
	print('FAILED: Response does not match expected!')
	print(diff)
	os.remove("../output")
	webserver.kill()
	exit(1)
else:
	print('SUCCESS!')
	os.remove("../output")
	webserver.kill()
	exit(0)
