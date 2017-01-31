import subprocess

print('Starting webserver...')
webserver = subprocess.Popen(['./webserver', 'config'], stdout=subprocess.DEVNULL)

print('Sending request...')
curl_proc = subprocess.Popen("curl -s localhost:8080", stdout=subprocess.PIPE, shell=True)

response = curl_proc.stdout.read().decode('utf-8')
expected = 'GET / HTTP/1.1\r\nUser-Agent: curl/7.51.0\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\n';

print('Checking response...')
if (response != expected):
	print('FAILED: Response does not match expected!')
	print('Expected: ' + expected)
	print('Response: ' + response)
	webserver.kill()
	exit(1)
else:
	print('SUCCESS')
	webserver.kill()
	exit(0)
