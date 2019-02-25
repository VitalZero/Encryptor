/*
 * Encryptor
 * File encryptor/decryptor.
 * Works on any kind of file.
 * Uses XOR encryption supplying a private key on a separate file
 * that very same key file is used for both encrypt and decrypt
 * example:
 * create a file named key.txt and inside of it, write these characters: mZq4t7w!
 * save the file and use it as a key to encrypt and decrypt any file
 *
 * For encryption:
 * Supply the "raw" file to be encrypted, use the key file and name the output file
 *
 * For decryption:
 * Supply the output file (encrypted), use the key file and the output file will be "raw" again.
 */
// TODO:
//Add a command line option to display outputs
#include <iostream>
#include <fstream>
#include <string>


// function that will encrypt/decrypt the file
// gets 3 args., input filename, key filename and output filename
int Encrypt(const std::string& file_in, const std::string& keyFile_in, const std::string& file_out);

int main(int argc, char** argv)
{
	// if user uses help, indicate the use of the program, exit.
	if(argc == 2)
	{
		if((argv[1][0] == '-') && (argv[1][1] == 'h'))
		{
			std::cout << "\nUso: " << argv[0] <<
					" \"in file\" \"file with key\" \"out file\"\n" << std::endl;;
		}
	}
	else if(argc == 4)
	{
		std::string input(argv[1]);
		std::string key(argv[2]);
		std::string output(argv[3]);
		// user typed 3 arguments, everything ok, do the magic (encryption/decryption)
		Encrypt(input, key, output);
	}
	else // if there's no arguments or the user typed more than 3 arguments,
		 //tell user to use help though -h option, exit.
	{
		std::cout << "\nExecute with " << argv[0] <<
				"-h for more information\n" << std::endl;
	}
	return 0;
}

int Encrypt(const std::string& file_in, const std::string& keyFile_in, const std::string& file_out)
{
	bool biggerThan5MB = false; //pending to implement

	// read input file
	std::ifstream inputFile(file_in, std::ios_base::in | std::ios_base::binary);
	if(!inputFile.is_open())
	{
		std::cerr << "Couldn't open file " << file_in << std::endl;
		return -1;
	}

	// go to end of file, get the position in bytes, and store it
	// the position in bytes will be the file size
	inputFile.seekg(0, std::ios_base::end);
	std::streamsize inputFileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios_base::beg); // go to beggining of file

	if(inputFileSize > (1024*1024) )
	{
		std::cout << "File size is " <<
			inputFileSize/1024/1000 << "," << (inputFileSize/1024)%1000 << " MB... " <<
			"Activating stream mode." << std::endl;

		biggerThan5MB = true;
	}

	// if the file is 1 byte in size or is empty, exit
	if(inputFileSize <= 1)
	{
		std::cerr << "File is empty or the size is really small (not worthy)" << std:: endl;
		return -2;
	}

	std::string inBuffer;
	inBuffer.resize(inputFileSize); // buffer to store the whole file contents in memory

	// read the whole file on the buffer
	inputFile.read((char*)&inBuffer[0], inputFileSize);
	inputFile.close();

	// read key file
	std::ifstream keyFile(keyFile_in, std::ios_base::in | std::ios_base::binary);
	if(!keyFile.is_open())
	{
		std::cerr << "Couldn't open file " << keyFile_in << std::endl;
		return -1;
	}

	// go to end of file, get the position in bytes, and store it
	// the position in bytes will be the file size
	keyFile.seekg(0, std::ios_base::end);
	std::streamsize keyFileSize = keyFile.tellg();
	keyFile.seekg(0, std::ios_base::beg); // go to beggining of file

	// buffer to hold the key characters found in the key file
	// read the key characters on the key buffer variable
	std::string keyFileBuffer;
	keyFileBuffer.resize(keyFileSize);
	keyFile.read( (char*)&keyFileBuffer[0], keyFileSize);
	keyFile.close();

	// output decryption/encryption
	std::cout << "\n\tStarting to do the magic" << std::endl;

	// do the XOR encryption
	// for each character in the buffer, XOR it using the key characters
	// use moddulus on the key character array using the key file size to avoid reading outside of array
	// example:
	//		i = 20 keyFileSize = 8 (8 bytes)
	//		i % keyFileSize = 4
	// character in the 5th position of the key array will be use to XOR the buffer character at 21th position
	// write the result in the same buffer
	int tick = inputFileSize / 15;
	std::cout << "Progress: ";
	for(int i = 0; i < inputFileSize; ++i)
	{
		inBuffer[i] = inBuffer[i] ^ keyFileBuffer[i%keyFileSize];
		if(i % tick == 0)
		{
			std::cout << "#";
		}
	}

	// open output file for write
	std::ofstream outFile(file_out, std::ios_base::out | std::ios_base::binary);
	if(!outFile.is_open())
	{
		std::cerr << "Couldn't open file " << file_out << std::endl;
		return -1;
	}

	// write the whole buffer chunk in the output file
	// as data was not added or removed, it is the same size as the input file
	for(int i = 0; i < inputFileSize; ++i)
	{
		outFile.put(inBuffer[i]);
		if(i % tick == 0)
		{
			std::cout << "#";
		}
	}
	std::cout << std::endl;
	//outFile.write( (char*)&inBuffer[0], inputFileSize);
	outFile.close();

	std::cout << "Finished!" << std::endl;

	return 0;
}

