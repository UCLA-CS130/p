import subprocess
import os

subprocess.call('cd ..', shell=True)

print('Starting webserver...')
webserver = subprocess.Popen(['./webserver', 'config'], stdout=subprocess.DEVNULL)
proxyserver = subprocess.Popen(['./webserver', 'proxy_integretation_test_config'], stdout=subprocess.DEVNULL)

print('TEST :Revrse Proxy Server Test')
print('Sending request...')
curl_proc = subprocess.Popen("curl -s localhost:8000", stdout=subprocess.PIPE, shell=True)

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
	