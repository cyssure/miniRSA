all:
	g++ -lpthread crackClient.cc Encryption.cpp Encrypt.cpp miniRSA.cpp -o crackClient
	g++ -lpthread crackServer.cc Encryption.cpp Encrypt.cpp miniRSA.cpp -o crackServer
	g++ crack.cpp Encryption.cpp Encrypt.cpp miniRSA.cpp -o crack
	g++ miniRSA.cpp Encryption.cpp Encrypt.cpp testEnDecrypt.cpp -o test
	g++ -lpthread server.cc Encrypt.cpp Encryption.cpp miniRSA.cpp -o server
	g++ -lpthread client.cc Encrypt.cpp Encryption.cpp miniRSA.cpp -o client