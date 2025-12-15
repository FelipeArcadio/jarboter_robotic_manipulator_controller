#include <PracticalSocket.cpp>
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()	#include <PracticalSockets.cpp>

using namespace std;

string servAddress = "localhost";
unsigned short echoServPort = 3000;
const int RCVBUFSIZE = 512;    // Size of receive buffer
int EnviarComando ( char echoString [ ], char echoBuffer [ ] );

int EnviarComando ( char echoString [ ], char echoBuffer [ ] )
{
	int echoStringLen = strlen(echoString);
	try 
	{
		// Establish connection with the echo server
		TCPSocket sock(servAddress, echoServPort);
	
		// Send the string to the echo server
		sock.send(echoString, echoStringLen);
	
		int bytesReceived = 0;              // Bytes read on each recv()
		int totalBytesReceived = 0;         // Total bytes read
		// Receive the same string back from the server
		while (totalBytesReceived < echoStringLen) 
		{
			// Receive up to the buffer size bytes from the sender
			if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0) 
			{
				cerr << "Unable to read";
				return 0;
			}
			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			echoBuffer[bytesReceived] = '\0';        // Terminate the string!
		}
		cout << endl;
		// Destructor closes the socket
	} 
	catch(SocketException &e) 
	{
		cerr << e.what() << endl;
		return 0;
	}
	return 1;

}

