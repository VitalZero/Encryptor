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
#include <cstdio>

// TODO:
//Add a command line option to display outputs

static bool consoleOutput = false;

// function that will encrypt/decrypt the file
// gets 3 args., input file, key file and output file
int Encrypt(const char* file_in, const char* keyFile_in, const char* file_out);

int main(int argc, char** argv)
{
	// if there's no arguments or the user typed more than 3 arguments, tell user to use help though -h option, exit.
	if( (argc == 1) || (argc > 3))
	{
		printf("\nExecute with %s -h for more information\n", argv[0]);
		return 0;
	}
	// if user uses help, indicate the use of the program, exit.
	else if(argc == 2)
	{
		if((argv[1][0] == '-') && (argv[1][1] == 'h'))
		{
			printf("\nUso: %s \"in file\" \"file with key\" \"out file\"\n", argv[0]);
			return 0;
		}
	}
	// user typed 3 arguments, everything ok, do the magic (encryption/decryption)
	Encrypt(argv[1], argv[2], argv[3]);

	return 0;
}

int Encrypt(const char* file_in, const char* keyFile_in, const char* file_out)
{
	//use 3 file pointers, one for input, one for key and other for output
	FILE* mainFile = nullptr;
	FILE* keyFile = nullptr;
	FILE* outFile = nullptr;

	char* inBuffer = nullptr; // buffer to store the whole file contents in memory
	char key[100]; // buffer to hold the key characters found in the key file **needs some future work**
	int mainFileSize = 0; // variable to hold the size of the input file size

	// read input file
	mainFile = fopen(file_in, "rb");

	// if can't open for read, close
	if(mainFile == nullptr)
	{
		printf("Couldn't open file %s!", file_in);
		return -1;
	}

	// go to end of file, get the position in bytes, and store it
	// the position in bytes will be the file size
	fseek(mainFile, 0, SEEK_END);
	mainFileSize = ftell(mainFile);
	rewind(mainFile); // go to beggining of file

	// if the file is 1 byte in size or is empty, exit
	if(mainFileSize <= 1)
	{
		printf("File is empty or the file is really small (not worthy)");
		return -2;
	}

	inBuffer = new char[mainFileSize]; // allocate memory for the file content

	if(inBuffer == nullptr)
	{
		printf("Couldn't allocate %d bytes of memory", mainFileSize);
		return -3;
	}

	// read the whole file on the buffer
	fread(inBuffer,sizeof(char), mainFileSize, mainFile);

	if(consoleOutput) //TODO: if this option is enabled, display the file contents
	{
		for(int i = 0; i < mainFileSize; i++)

		{
			putchar(inBuffer[i]);
		}
		puts("");
	}

	fclose(mainFile);

	// read key file
	keyFile = fopen(keyFile_in, "rb");
	// if can't open for read, close
	if(keyFile == nullptr)
	{
		printf("Couldn't open file %s!", keyFile_in);
		return -1;
	}

	// go to end of file, get the position in bytes, and store it
	// the position in bytes will be the file size
	fseek(keyFile, 0, SEEK_END);
	const int keyFileSize = ftell(keyFile);
	rewind(keyFile); // go to beggining of file

	// read the key characters on the key buffer variable
	fread(key, sizeof(char), keyFileSize, keyFile);

	if(consoleOutput) //TODO: if this option is enabled, display the file contents
	{
		for(int i = 0; i < keyFileSize; i++)
		{
			putchar(key[i]);
		}
		printf("\nSize: %i", keyFileSize);
	}

	fclose(keyFile);

	// output decryption/encryption
	puts("\n\tStarting to do the magic\n");

	// do the XOR encryption
	// for each character in the buffer, XOR it using the key characters
	// use moddulus on the key character array using the key file size to avoid reading outside of array
	// example:
	//		i = 20 keyFileSize = 8 (8 bytes)
	//		i % keyFileSize = 4
	// character in the 5th position of the key array will be use to XOR the buffer character at 21th position
	// write the result in the same buffer
	for(int i = 0; i < mainFileSize; ++i)
	{
		inBuffer[i] = inBuffer[i] ^ key[i%keyFileSize];
	}

	if(consoleOutput) //TODO: if this option is enabled, display the file contents
	{
		for(int i = 0; i < mainFileSize; i++)
		{
			putchar(inBuffer[i]);
		}
		puts("");
	}

	// open output file for write
	outFile = fopen(file_out, "wb");
	// if can't open, exit
	if(outFile == nullptr)
	{
		printf("Couldn't open file %s!", file_out);
		return -1;
	}

	// write the whole buffer chunk in the output file
	// as data was not added or removed, it is the same size as the input file
	fwrite(inBuffer, sizeof(char), mainFileSize, outFile);

	fclose(outFile);

	// clean up the buffer
	delete[] inBuffer;
	puts("Finished!");

	// bye bye!!
	return 0;
}

