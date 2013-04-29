all:
	g++ -lpthread crackClient.cc Encryption.cpp Encrypt.cpp miniRSA.cpp -o crackClient
	g++ -lpthread crackServer.cc Encryption.cpp Encrypt.cpp miniRSA.cpp -o crackServer
	g++ crack.cpp Encryption.cpp Encrypt.cpp miniRSA.cpp -o crack
	g++ Encryption.cpp miniRSA.cpp encryptiontest.cpp -o encrytest
	g++ miniRSA.cpp Encryption.cpp Encrypt.cpp testEnDecrypt.cpp -o endecrypt
	g++ -lpthread server.cc Encrypt.cpp Encryption.cpp miniRSA.cpp -o server
	g++ -lpthread client.cc Encrypt.cpp Encryption.cpp miniRSA.cpp -o client
run:
	./server 8000
clean:
	rm crackClient crackServer crack encrytest endecrypt server client