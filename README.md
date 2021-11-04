Welcome to a brief manual on how to operate my proxy, before doing so please ensure that you are
running this program while using a Linux system (for example, you could use remote-SSH to connect to
the University of Calgary Linux servers). While you can operate this proxy on Chrome, I personally used
FireFox for the testing of this proxy and would encourage the user to do the same.



Steps to Operate

1. Firstly, you want to begin by installing the C file I have submitted (proxy.c)
a. I would recommend clearing the cache in FireFox (open settings, enter cache into the
search bar in settings, click on clear data, then ok). This is an optional step.
2. Secondly, you want to use the “ifconfig” command to get the IP of your machine. Once you enter
ifconfig, three large bodies of texts separated by a space should appear, go to the very first body
of text and save the inet IP in the second line.
3. Then open up FireFox, go to settings (which can be found by clicking on the three horizontal line
symbol on the far right, next to the URL), then scroll all the way down until you see “Network
Settings”, clicking on it should open up a connection settings page where you can configure the
browser’s proxy access. Click on manual proxy configuration (last bullet) and fill in the IP
retrieved from your machine (the one we got using ifconfig) and put that into the HTTP Proxy
bar. Keep this page open as we still need to fill in the port number.
4. Afterwards please use a C compiler (I personally used gcc), if you decide to use gcc, use the line
below to compile the C file into an executable file
a. gcc -o proxy.c proxy
i. Please note that you can name the executable file whatever you want, I decided to
name it proxy for convenience sake
5. After you’ve successfully compiled the C file into an executable use the line below to execute
a. ./proxy [PORT NUMBER], where [PORT NUMBER] is the port number you manually
enter (ex. ./proxy 8000), in this example I am using port 8000.
6. Once you’ve entered a port number and are running the proxy, you should see three lines of text
appear in your terminal informing you that a socket has been made, binding was successful and
now it’s waiting for clients. Now return to the proxy configuration page we left open from step
#3 and fill in the port number you used as the command line argument next to the IP you entered
previously.
7. Once you have entered both the IP and port, click ‘OK’ which should close the page.
8. Once you’ve done this, select any of the test pages provided for us and you should see a
“Connection Accepted” line appear in the terminal and you can freely browse all test cases and
the links provided which redirect to either other HTML pages of JPEGs.
9. To exit out of the proxy, simply go to the terminal and type ctrl + Z.
